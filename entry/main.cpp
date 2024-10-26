#include <omtl/Language.hpp>
#include <sstream>

int main(int argc, char* argv[]) {
    omtl::MacroParser mp;
    omtl::Tokenizer t;
    omtl::ParseTreeBuilder ptb;

    std::stringstream fakefile{"hello: 100, \"str\", 200, \"str2\", hello, 2 + 3"};

    auto tokens = t.tokenize(fakefile, "testfile");
    auto parseTree = ptb.buildParseTree(tokens);
    mp.parseGlobal(parseTree);

    for(auto e: mp.globalScope){
        std::cout << e.first << " " << e.second.object_id << "\n";
    }

    // for(auto e: omtl::stringLiteralMap){
    //     std::cout << e.first << " " << e.second << "\n";
    // }

    // for(auto e: omtl::numericLiteralMap){
    //     std::cout << e.first << " " << e.second << "\n";
    // }
    
    // //give it a directory, or a file
    // omtl::FunctionArguments f = {{"key1", omtl::Object{}}, {"key2", omtl::Object{}}};
    // omtl::Object o{
    //     {},
    //     [](auto args) { return omtl::Object{}; },
    // };
    return 0;
}