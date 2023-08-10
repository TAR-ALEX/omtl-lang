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

    static OmtlObject makeType(std::string s) { // delete this.
        OmtlObject v;
        v.type = s;
        return v;
    }

    virtual std::string toString() {
        std::stringstream ss;

        for (auto& [k, v] : publicMembers) { ss << v->toString() << std::endl; }
        for (auto& [k, v] : privateMembers) { ss << v->toString() << std::endl; }
        return name + "{" + type + "} [\n" + estd::string_util::indent(ss.str(), "  ") + "]";
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

    bool checkMatches(OmtlObject o) {}

    void call(OmtlObject passed) {}

    OmtlFunction(std::string argumentMatcher, std::function<void(OmtlObject)>) {
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

                    //  std::cout << k << ": " << v.getName() << "\n";// get raw?
                } else {
                    if (!v.isTuple()) {
                        std::cerr << v.getDiagnosticString() << std::endl;
                        exit(0);
                        throw std::runtime_error("Invalid type");
                    }
                    writeTo->insert(OmtlObject{"", k});
                    parseMatcher(&(writeTo->at(k)->publicMembers), v);
                }
            }
        };

        parseMatcher(&argTemplate.publicMembers, *elements[0][0]);
    }

    bool operator<(const OmtlFunction& right) const { // needed for sorting in a set
        return arg < right.arg;
    }

    std::string toString() { return argTemplate.toString(); }
};

struct OmtlFunctor : OmtlObject {
    OmtlFunctor(std::string name, std::set<OmtlFunction> funcs) {}
};