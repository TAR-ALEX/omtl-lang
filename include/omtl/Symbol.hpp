#pragma once

#include <functional>
#include <map>
#include <omtl/ParseTree.hpp>
#include <string>

namespace omtl {
    class Macro;
    class Function;
    //typedef std::deque<std::string> FunctionSignature;
    typedef std::deque<std::pair<std::string, Macro>> FunctionArguments;

    class Macro {
    public:
        std::string macro_id; 
        std::map<std::string, Macro> members;
        std::function<Macro(FunctionArguments)> onCall;

        Macro(
            std::map<std::string, Macro> members,
            std::function<Macro(FunctionArguments)> onCall
        );

        Macro(std::function<Macro(FunctionArguments)> onCall);
        Macro();

        Macro call(FunctionArguments passedArgs);
    };
}; // namespace omtl