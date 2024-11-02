#include <omtl/Language.hpp>
#include <sstream>

int main(int argc, char* argv[]) {
    omtl::MacroParser mp;
    omtl::Tokenizer t;
    omtl::ParseTreeBuilder ptb;

    std::stringstream fakefile{"hello: 100, \"str\", th: 200, \"str2\", hello, th +"};

    auto tokens = t.tokenize(fakefile, "testfile");
    auto parseTree = ptb.buildParseTree(tokens);
    mp.parseGlobal(parseTree);

    for(auto e: mp.globalScope){
        std::cout << e.first << " " << e.second.macro_id << "\n";
    }

    // for(auto e: omtl::stringLiteralMap){
    //     std::cout << e.first << " " << e.second << "\n";
    // }

    // for(auto e: omtl::numericLiteralMap){
    //     std::cout << e.first << " " << e.second << "\n";
    // }
    
    return 0;
}