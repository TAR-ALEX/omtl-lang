#pragma once

// #include <AutonameMap.h>
// #include <deque>
// #include <estd/ptr.hpp>
// #include <functional>
// #include <iostream>
// #include <map>
// #include <omtl/ParseTree.hpp>
// #include <omtl/Tokenizer.hpp>
// #include <sstream>

// using namespace estd::shortnames;

// struct Object;
// struct StringObject;
// struct NumericObject;

// cptr<Object> argTemplateToObject (std::string argumentMatcher) {
//     cptr<Object> argTemplate{"Tuple", "arguments"};

//     std::stringstream sfstream(argumentMatcher);
//     auto tokens = omtl::Tokenizer{}.tokenize(sfstream, "private");
//     auto elements = omtl::ParseTreeBuilder{}.buildParseTree(tokens);

//     std::function<void(AutonameMap<cptr<Object>> * writeTo, omtl::Element e)> parseMatcher;
//     parseMatcher = [&](AutonameMap<cptr<Object>>* writeTo, omtl::Element e) {
//         for (auto& [k, v] : e) {
//             if (v.isName()) {
//                 if (v.getName() == "String") {
//                     writeTo->insert(new StringObject{k});
//                 } else if (v.getName() == "Number") {
//                     writeTo->insert(new NumericObject{k});
//                 } else {
//                     throw std::runtime_error("Unknown type " + v.getName());
//                 }
//             } else if (v.isStatement() && v[0]->isTuple()) {
//                 writeTo->insert(Object{"Tuple", k});
//                 parseMatcher(&(writeTo->at(k)->publicMembers), v[0].value());
//             } else {
//                 // std::cerr << v.getDiagnosticString() << std::endl;
//                 throw std::runtime_error("Invalid type");
//             }
//         }
//     };
//     parseMatcher(&argTemplate->publicMembers, *elements[0][0]); // root code's first statment
// }