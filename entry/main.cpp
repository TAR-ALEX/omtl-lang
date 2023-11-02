#include "AutonameMap.h"
#include <deque>
#include <estd/ptr.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <omtl/ParseTree.hpp>
#include <omtl/Tokenizer.hpp>
#include <sstream>
#include <omtl/language.h>

using namespace estd::shortnames;

class OmtlASTbuilder {
public:
    estd::ostream_proxy log{&std::cout};

    // std::deque<AutonameMap<cptr<OmtlObject>>> scope = {{
    //     {
    //         "class",
    //         "String",
    //         {
    //             OmtlFunctor{
    //                 "+",
    //                 {
    //                     OmtlFunction{
    //                         "[x: String, y: Number]",
    //                         [](OmtlObject parameters) {},
    //                     },
    //                 },
    //             },
    //         },
    //     },
    // }};

    void build(omtl::Element input) {
        for (auto [name, element] : input) {}
    }

    std::string dump() {
        std::stringstream ss;
        ss << "test";
        return ss.str();
    }
};

int main() {
    auto tokens = omtl::Tokenizer{}.tokenize("./text/in.omtl");
    //for (auto& tok : tokens) std::cout << tok.getDiagnosticString() << std::endl;
    auto rootElement = omtl::ParseTreeBuilder{}.buildParseTree(tokens);
    std::cout << rootElement.getDiagnosticString() << std::endl;

    OmtlASTbuilder compiler;
    compiler.build(rootElement);
    std::cout << compiler.dump() << std::endl;
    return 0;
}