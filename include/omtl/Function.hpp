#pragma once

#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>

#include <estd/ptr.hpp>

#include <omtl/ParseTree.hpp>
#include <omtl/Tokenizer.hpp>
#include <omtl/AutonameMap.hpp>
#include <omtl/StringObject.hpp>
#include <omtl/NumericObject.hpp>

namespace omtl {

    using namespace estd::shortnames;

    // This cannot be declared directly in code, this is only for internal representation, objects call these functions internally
    struct Function { // notice that this is not an object
        std::string arg;
        Object argTemplate{"Tuple", "arguments"};
        std::function<void(Object*)> impl;

        void call(Object* passed) {
            std::function<bool(Object*, Object*)> isSame;
            isSame = [&](Object* left, Object* right) {
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

        Function(std::string argumentMatcher, std::function<void(Object*)> f) {
            std::stringstream sfstream(argumentMatcher);
            auto tokens = omtl::Tokenizer{}.tokenize(sfstream, "private");
            auto elements = omtl::ParseTreeBuilder{}.buildParseTree(tokens);
            arg = elements.getDiagnosticString();
            std::function<void(AutonameMap<cptr<Object>> * writeTo, omtl::Element e)> parseMatcher;
            parseMatcher = [&](AutonameMap<cptr<Object>>* writeTo, omtl::Element e) {
                for (auto& [k, v] : e) {
                    if (v.isName()) {
                        if (v.getName() == "String") {
                            writeTo->insert(new StringObject{k});
                        } else if (v.getName() == "Number") {
                            writeTo->insert(new NumericObject{k});
                        } else {
                            throw std::runtime_error("Unknown type " + v.getName());
                        }
                    } else if (v.isStatement() && v[0]->isTuple()) {
                        writeTo->insert(Object{"Tuple", k});
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

        bool operator<(const Function& right) const { // needed for sorting in a set, used by functor
            return arg < right.arg;
        }

        std::string toString() { return argTemplate.toString(); }
    };

    struct Functor : Object {
        Functor(std::string name, std::set<Function> funcs) {}
    };

}; // namespace omtl