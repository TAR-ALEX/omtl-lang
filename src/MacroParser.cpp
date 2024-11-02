#include <omtl/MacroParser.hpp>

namespace omtl {
    omtl::Macro MacroParser::parseSingleElement(omtl::Element elem, ScopeType& scope) {
        if (elem.isNumber()) {
            return NumberLiteral(elem.getNumber());
        } else if (elem.isString()) {
            return StringLiteral(elem.getString());
        } else if (elem.isName()) {
            return scope.at(elem.getName());
        }
        return omtl::NullMacro(); // null macro return
    }

    void MacroParser::parseGlobal(omtl::Element tree) {
        for (auto& elem : tree) {
            if(elem.second.isToken()){
                globalScope.insert({elem.first, parseSingleElement(elem.second, globalScope)});
            }else {
                globalScope.insert({elem.first, parseStatement(elem.second, globalScope)});
            }
            // method 1:
            // try to define as many objects as reasonably possible.
            // go back and find recursive definitions and fix them.
            
            // this only applies to recursive members, functor elements 
            // can be defined at any time. we care about the members here

            // method 2 (better)
            // build a list of all names in the root tuple, matched with their element tree,
            // when done, go through all tuple elements, even unnamed ones, and implement their macros.
            // if a macro needs another macro that doesnt exist. find it in the name list and implement it on the fly.
            // we can use pointers to resolve the issue with recursive loops overflowing the stack.
            // detect crazy recursive loops later during code generation, because on the macro level 
            // pointers would cause issues.

            // there is no notion of stack/heap here, everything is just a flat global tree in terms of macros.
            // there will be special codegen macros to manage the stack and heap. perhaps an llvm codegen class 
            // or direct assembly.
        }
    }

    omtl::Macro MacroParser::parseStatement(omtl::Element statement, ScopeType& scope) {
        omtl::Macro currentMacro = parseSingleElement(statement[0], scope);
        for (size_t i = 1; i < statement.size(); i++) {
            if(statement[i].isName()){
                std::string name = statement[i].getName();
                if(currentMacro.members.count(name) != 0){
                    currentMacro = currentMacro.members[name];
                    continue;
                }else{
                    // pass it to the functor
                    throw std::runtime_error("not implemented yet");
                }
            }else{
                // pass to the functor
                throw std::runtime_error("not implemented yet");
            }
        }
        return currentMacro;
    }
    void MacroParser::parseTuple(omtl::Element tuple, ScopeType& scope) {

    }
}; // namespace omtl