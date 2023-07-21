// #include "llvm/ExecutionEngine/ExecutionEngine.h"
// #include "llvm/ExecutionEngine/GenericValue.h"
// #include "llvm/ExecutionEngine/MCJIT.h"
// #include "llvm/IR/IRBuilder.h"
// #include "llvm/IR/ValueSymbolTable.h"
// #include "llvm/IR/Verifier.h"
// #include "llvm/Support/TargetSelect.h"
#include "AutonameMap.h"
#include <deque>
#include <estd/ptr.hpp>
#include <iostream>
#include <map>
#include <omtl/ParseTree.hpp>
#include <omtl/Tokenizer.hpp>

using namespace estd::shortnames;

// Some declarations for future use.
struct OmtlObject;
struct OmtlFunction;

// This cannot be declared directly in code, this is only for internal representation, objects call these functions internally
struct OmtlFunction {
    // rptr<llvm::Value> val = nullptr;
};


struct OmtlArgumentMatcher {
    // matches tuple lists that are passed to and object.
    std::map<std::string, OmtlArgumentMatcher> m;
    sptr<std::string> datatype = nullptr;
    // OmtlArgumentMatcher(std::string datatype) : datatype(datatype) {}
    // OmtlArgumentMatcher(const char* datatype) : datatype(datatype) {}
    // OmtlArgumentMatcher(std::initializer_list<std::pair<std::string, OmtlArgumentMatcher>> internal);
    OmtlArgumentMatcher(std::string s) {
        std::stringstream ss{s};
        auto tokens = omtl::Tokenizer{}.tokenize(ss, "OmtlArgumentMatcher");
        auto rootElement = omtl::ParseTreeBuilder{}.buildParseTree(tokens);
        // for(auto & element : rootElement) {

        // }
    }
};

// OmtlArgumentMatcher::OmtlArgumentMatcher(std::initializer_list<std::pair<std::string, OmtlArgumentMatcher>> internal) {
//     for (auto [key, value] : internal) { m.insert_or_assign(key, value); }
// }

// OmtlArgumentMatcher matcher({{"arg1", "integer"}, {"arg2", "integer"}});

struct OmtlObject {
    // rptr<llvm::Value> val = nullptr;

    sptr<estd::BigDecimal> num = nullptr;
    sptr<std::string> str = nullptr;


    std::string name;
    std::string type;

    cptr<OmtlObject> parent; // useful for functions that need access to parent (instance) vars. aka, object to the left
    AutonameMap<OmtlObject> members; // member variables and functions.

    // Objects can take arguments. in tuples or single
    // Without the tuple syntax there can be some ambiguity. (a possible solution is to not allow parameters without tuples if there are member variables)


    OmtlObject() {}
    OmtlObject(std::nullptr_t) {}
    OmtlObject(estd::BigDecimal n) { num = n; }
    OmtlObject(std::string type, std::string name, AutonameMap<OmtlObject> members = {}) :
        name(name), type(type), members(members) {}
    static OmtlObject makeType(std::string s) {
        OmtlObject v;
        v.type = s;
        return v;
    }
};

class OmtlASTbuilder {
public:
    estd::ostream_proxy log{&std::cout};

    std::deque<AutonameMap<OmtlObject>> scope = {{
        {"operator", "+"},
        {"operator", "+="},
        {"operator", "-"},
        {"class",
         "Int",
         {
             {"[Int]function", "+"},
             {"function", "-"},
         }},
    }};

    void build(omtl::Element e) {
        log << "compiling entry\n";
        declareBlock(e);
    }
    OmtlObject getVarType(std::string name) {
        for (auto sc = scope.rbegin(); sc != scope.rend(); ++sc) {
            if (sc->count(name)) return sc->at(name);
        }
        // dumpScope();
        throw std::runtime_error(name + " does not exist.");
    }
    OmtlObject getType(cptr<omtl::Element> e) {
        if (e->isStatement()) { return getStatementType(e); }
        if (e->isValue()) { return getValueType(e); }
        if (e->isTuple()) { return getTupleType(e); }
        throw std::runtime_error("Unknown element at " + e->location);
    }
    OmtlObject getValueType(cptr<omtl::Element> e) {
        if (!e->isValue()) { throw std::runtime_error("expected value at " + e->location); }
        if (e->isNumber()) return OmtlObject::makeType("Constant");
        else if (e->isString())
            return OmtlObject::makeType("String");
        else if (e->isName()) { return getVarType(e->getName()); }
        throw std::runtime_error("Unknown element at " + e->location);
    }
    OmtlObject getTupleType(cptr<omtl::Element> e) {
        if (!e->isTuple()) { throw std::runtime_error("expected tuple at " + e->location); }
        if (e->size() > 1) {
            declareBlock(e);
            return OmtlObject::makeType("Block");
        } else if (e->size() == 1) {
            return getStatementType(e[0]);
        } else {
            return OmtlObject::makeType("null");
        }
    }
    OmtlObject getStatementType(cptr<omtl::Element> e) {
        if (!e->isStatement()) { throw std::runtime_error("expected statement at " + e->location); }

        auto getGreatestType = [](std::string t1, std::string t2) { return t1; };

        std::string t1 = "";
        std::string t2 = "";

        // omtl::Element accumulator = omtl::Statement({});
        std::deque<OmtlObject> accumulator;

        while (e->size() != 0) {
            omtl::Element elem = e->popFront();
            accumulator.push_back(getType(elem));
            if (accumulator.size() >= 3) {
                if (accumulator[1].type == "operator") { // is operator, also needed to check if the type is primitive.
                    t1 = accumulator[0].type;
                    t2 = accumulator[2].type;
                    for (size_t i = 0; i < 3; i++) accumulator.pop_front();
                    accumulator.push_front(OmtlObject::makeType(getGreatestType(t1, t2)));
                }
            }
            if (accumulator.size() >= 2) {
                if (accumulator[0].type == "class") { // is class
                    t1 = accumulator[0].name;
                    for (size_t i = 0; i < 2; i++) accumulator.pop_front();
                    accumulator.push_front(OmtlObject::makeType(t1));
                }
            }
        }
        if (accumulator.size() != 1) {
            throw std::runtime_error("bad statment at " + e->location + " " + std::to_string(accumulator.size()));
        }
        return accumulator[0];
    }
    void declareVar(std::string name, cptr<omtl::Element> statement) {
        log << "declaring var " + name + "\n";
        OmtlObject v;
        v.type = getStatementType(statement).type;
        scope.back()[name] = v;
    }
    void declareBlock(cptr<omtl::Element> block) {
        log << "declaring block\n";
        scope.push_back({});
        for (size_t i = 0; i < block->size(); i++) {
            auto statement = block[i];
            auto label = block->getLabel(i);
            if (label == "") label = std::to_string(i);
            declareVar(label, statement);
        }
        dumpScope();
        scope.pop_back();
    }
    OmtlObject compileStatement(cptr<omtl::Element> statement) {
        log << "compiling statement\n";
        std::cout << "line " << statement->getDiagnosticString() << "\n";
        if (statement[0]->isNumber()) { return compileConst(statement); }
        return nullptr;
    }
    OmtlObject compileConst(cptr<omtl::Element> elem) {
        log << "compiling const\n";
        if (elem->size() == 1) return OmtlObject(elem[0]->getNumber());
        else if (elem->size() == 3) {
            auto op = elem[1]->getName();
            estd::BigDec newVal = elem[0]->getNumber();
            if (op == "+") { newVal = elem[0]->getNumber() + elem[2]->getNumber(); }
            return newVal;
        }
        throw std::runtime_error("bad const " + elem->location);
    }
    void dump() {}
    void dumpScope() {
        log << "-------------------------------\n";
        for (auto sc = scope.rbegin(); sc != scope.rend(); ++sc) {
            log << "-------------\n";
            for (auto& [name, v] : *sc) { log << name << " | " << v.type << "\n"; }
        }
        log << "-------------------------------\n";
    }
};

int main() {
    auto tokens = omtl::Tokenizer{}.tokenize("./text/in.omtl");
    // for (auto& tok : tokens) std::cout << tok.getDiagnosticString() << std::endl;
    auto rootElement = omtl::ParseTreeBuilder{}.buildParseTree(tokens);
    // std::cout << rootElement.getDiagnosticString() << std::endl;

    OmtlASTbuilder compiler;
    compiler.build(rootElement);
    compiler.dump();
    return 0;
}