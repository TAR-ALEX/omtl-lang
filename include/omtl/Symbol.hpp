#pragma once

#include <functional>
#include <map>
#include <omtl/ParseTree.hpp>
#include <string>

namespace omtl {
    class Object;
    class Function;
    //typedef std::deque<std::string> FunctionSignature;
    typedef std::deque<std::pair<std::string, Object>> FunctionArguments;

    class Object {
    public:
        std::string object_id; 
        std::map<std::string, Object> members;
        std::function<Object(FunctionArguments)> onCall;

        Object(
            std::map<std::string, Object> members,
            std::function<Object(FunctionArguments)> onCall
        );

        Object(std::function<Object(FunctionArguments)> onCall);
        Object();

        Object call(FunctionArguments passedArgs);
    };
}; // namespace omtl