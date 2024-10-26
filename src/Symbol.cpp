#include <omtl/Symbol.hpp>

namespace{
    int64_t symbolID = 0;
    std::string getNextSymbolID(){
        std::stringstream ss;
        ss << "symbol_" << symbolID;
        symbolID++;
        return ss.str();
    }
};

namespace omtl {

    Object Object::call(FunctionArguments passedArgs) {
        return onCall(passedArgs);
    }

    Object::Object(
        std::map<std::string, Object> members,
        std::function<Object(FunctionArguments)> onCall = [](FunctionArguments){throw std::runtime_error("onCall is undefined"); return omtl::Object{};}
    ) : object_id(getNextSymbolID()), members(members), onCall(onCall) {}

    Object::Object(
        std::function<Object(FunctionArguments)> onCall
    ) : object_id(getNextSymbolID()), members({}), onCall(onCall) {}

    Object::Object() : object_id(getNextSymbolID()), onCall([](FunctionArguments){throw std::runtime_error("onCall is undefined"); return omtl::Object{};}) {}
}; // namespace omtl