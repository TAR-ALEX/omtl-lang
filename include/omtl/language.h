#include <AutonameMap.h>
#include <deque>
#include <estd/ptr.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <omtl/ParseTree.hpp>
#include <omtl/Tokenizer.hpp>
#include <sstream>

using namespace estd::shortnames;
// Some declarations for future use.
struct OmtlObject;
struct OmtlFunction;

// omtl object is an object or an overloadable function like object
struct OmtlObject : estd::clonable {
    std::string name; // var name, should not be changed after adding it to an AutonameMap
    // The only true reason we need a name here is for diagnostic purposes. The name used in the map is what really matters.
    std::string type; // name of class that created this.

    rptr<OmtlObject> parent; // useful for functions that need access to parent (instance) vars.
    // aka, object to the left using the dot syntax in C++ like objects. So far, only functors should use this.

    AutonameMap<cptr<OmtlObject>> privateMembers; // member variables and functions.
    AutonameMap<cptr<OmtlObject>> publicMembers;

    // Objects can take arguments. in tuples or by passing a single object to its right.
    // Without the tuple syntax there can be some ambiguity. (a possible solution is to not allow parameters without tuples if there are member variables)
    // This basically makes the functors a special case.
    OmtlObject() {}
    OmtlObject(
        std::string type,
        std::string name,
        AutonameMap<cptr<OmtlObject>> publicMembers = {},
        AutonameMap<cptr<OmtlObject>> privateMembers = {}
    ) :
        name(name),
        type(type), privateMembers(privateMembers), publicMembers(publicMembers) {}


    virtual clonable* clone() const { return new OmtlObject(*this); }

    virtual std::string toString() {
        std::stringstream ss;

        for (auto& [k, v] : publicMembers) { ss << v->toString() << std::endl; }
        for (auto& [k, v] : privateMembers) { ss << v->toString() << std::endl; }
        return name + "{" + type + "}: [\n" + estd::string_util::indent(ss.str(), "  ") + "]";
    }
};

struct OmtlNullObject : public OmtlObject {
    OmtlNullObject() { type = "Null"; }
    virtual std::string toString() { return "Null"; }

    virtual clonable* clone() const { return new OmtlNullObject(*this); }
};

struct OmtlStringObject : public OmtlObject {
    std::string data;

    OmtlStringObject(std::string name, std::string data = "") : data{data} {
        this->name = name;
        this->type = "String";
    }
    virtual std::string toString() { return name + "{" + type + "}: " + data; }

    virtual clonable* clone() const { return new OmtlStringObject(*this); }
};

struct OmtlNumericObject : public OmtlObject {
    estd::BigDecimal data;

    OmtlNumericObject(std::string name, estd::BigDecimal d = 0) : data{d} {
        this->name = name;
        this->type = "Number";
    }
    virtual std::string toString() { return name + "{" + type + "}: " + data.toString(); }

    virtual clonable* clone() const { return new OmtlNumericObject(*this); }
};

// This cannot be declared directly in code, this is only for internal representation, objects call these functions internally
struct OmtlFunction { // notice that this is not an object
    std::string arg;
    OmtlObject argTemplate{"Tuple", "arguments"};
    std::function<void(OmtlObject*)> impl;

    void call(OmtlObject* passed) {
        std::function<bool(OmtlObject*, OmtlObject*)> isSame;
        isSame = [&](OmtlObject* left, OmtlObject* right) {
            if (left->type != right->type) return false;
            if (left->type == "Tuple") {
                if (left->publicMembers.size() != right->publicMembers.size()) { return false; }
                for (auto& [k, v1] : left->publicMembers) {
                    if (right->publicMembers.count(k) == 0) return false;
                    auto& v2 = right->publicMembers[k];
                    if (!isSame(v1.get(), v2.get())) { return false; }
                }
            }

            return true;
        };
        if (isSame(passed, &argTemplate)) {
            impl(passed);
        } else {
            throw std::runtime_error("Cannot call function argument mismatch!");
        }
    }

    OmtlFunction(std::string argumentMatcher, std::function<void(OmtlObject*)> f) {
        std::stringstream sfstream(argumentMatcher);
        auto tokens = omtl::Tokenizer{}.tokenize(sfstream, "private");
        auto elements = omtl::ParseTreeBuilder{}.buildParseTree(tokens);
        arg = elements.getDiagnosticString();
        std::function<void(AutonameMap<cptr<OmtlObject>> * writeTo, omtl::Element e)> parseMatcher;
        parseMatcher = [&](AutonameMap<cptr<OmtlObject>>* writeTo, omtl::Element e) {
            for (auto& [k, v] : e) {
                if (v.isName()) {
                    if (v.getName() == "String") {
                        writeTo->insert(new OmtlStringObject{k});
                    } else if (v.getName() == "Number") {
                        writeTo->insert(new OmtlNumericObject{k});
                    } else {
                        throw std::runtime_error("Unknown type " + v.getName());
                    }
                } else if (v.isStatement() && v[0]->isTuple()) {
                    writeTo->insert(OmtlObject{"Tuple", k});
                    parseMatcher(&(writeTo->at(k)->publicMembers), v[0].value());
                } else {
                    // std::cerr << v.getDiagnosticString() << std::endl;
                    throw std::runtime_error("Invalid type");
                }
            }
        };
        parseMatcher(&argTemplate.publicMembers, *elements[0][0]); // root code's first statment
        impl = f;
    }

    bool operator<(const OmtlFunction& right) const { // needed for sorting in a set, used by functor
        return arg < right.arg;
    }

    std::string toString() { return argTemplate.toString(); }
};

struct OmtlFunctor : OmtlObject {
    OmtlFunctor(std::string name, std::set<OmtlFunction> funcs) {}
};