#include <deque>
#include <estd/ptr.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <omtl/ParseTree.hpp>
#include <omtl/Tokenizer.hpp>
#include <sstream>
#include <omtl/Language.hpp>

using namespace estd::shortnames;

int main() {
    auto tokens = omtl::Tokenizer{}.tokenize("./text/classSample.omtl");
    for (auto& tok : tokens) std::cout << tok.getDiagnosticString() << std::endl;
    auto rootElement = omtl::ParseTreeBuilder{}.buildParseTree(tokens);
    std::cout << rootElement.getDiagnosticString() << std::endl;

    omtl::CodeGen compiler;
    compiler.setParseTree(rootElement);
    compiler.codeGenPass1();
    // compiler.build(rootElement);
    // std::cout << compiler.dump() << std::endl;
    return 0;
}