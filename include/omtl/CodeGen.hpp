#pragma once

#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>

#include <estd/ptr.hpp>

#include <omtl/ParseTree.hpp>
#include <omtl/Tokenizer.hpp>

#include <omtl/Util.hpp>

#include <omtl/Function.hpp>
#include <omtl/NumericObject.hpp>
#include <omtl/Object.hpp>
#include <omtl/StringObject.hpp>

namespace omtl {
    class CodeGen {
        estd::clone_ptr<omtl::Element> parseTree;

        std::deque<omtl::Object> structures;

    public:
        CodeGen() {}
        CodeGen(estd::clone_ptr<omtl::Element> parseTree) { setParseTree(parseTree); }
        void setParseTree(estd::clone_ptr<omtl::Element> parseTree) { this->parseTree = parseTree; }

        void buildClass(std::string name, estd::clone_ptr<omtl::Element> element) {
            using namespace estd::shortnames;
            if(element[0]->getName() != "class" || !element[1]->isTuple() || element->size() != 2) {
                throw std::runtime_error("class declaration is invalid at " + element->location);
            }
            cptr<omtl::Element> rootTuple = element[1];
            cptr<omtl::Element> publicE = rootTuple["public"][0];
            cptr<omtl::Element> privateE = rootTuple["private"][0];
            cptr<omtl::Element> constructor = rootTuple["constructor"][0];
            cptr<omtl::Element> destructor = rootTuple["destructor"][0];
            
            omtl::Object instance;
            instance.type = name;
            for (auto [name, decl] : publicE){
                //std::cout << decl.getDiagnosticString() << std::endl;
                if(decl[0]->getName() == "Int") {
                    if (decl[1]->getNumber() == 64){
                        instance.publicMembers.insert(cptr<omtl::NumericObject>(name, decl[2]->getNumber()));
                    }else{
                        throw std::runtime_error("Unsupported int size at " + decl.location);
                    }
                }
            }

            std::cout << instance.cppImplementStructure() << std::endl;

            //std::cout << publicE->getDiagnosticString() << std::endl;
        }

        void codeGenPassRecursive(estd::clone_ptr<omtl::Element> rootElem) {
            for (auto [name, element] : rootElem) {
                // std::cout << name << std::endl;
                if (!element.isStatement()) {
                    throw std::runtime_error(
                        "expected a statment at " + element.location + " maybe this is not a tuple?"
                    );
                }
                if (element[0]->getName() == "class") {
                    std::cout << "found class" << std::endl;
                    buildClass(name, element);
                }
            }
        }

        // Parses the code to find all structure names and definitions, builds a table for the structure names
        // After finishing the parsing, sorts the structure definitions in the most beneficial way to facilitate composition.
        //
        void codeGenPass1() { codeGenPassRecursive(parseTree); }

        std::string createStructureDeclarations() {}

        std::string createStructureImplementations() {}
    };
}; // namespace omtl