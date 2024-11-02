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

    Macro Macro::call(FunctionArguments passedArgs) {
        return onCall(passedArgs);
    }

    Macro::Macro(
        std::map<std::string, Macro> members,
        std::function<Macro(FunctionArguments)> onCall = [](FunctionArguments){throw std::runtime_error("onCall is undefined"); return omtl::Macro{};}
    ) : macro_id(getNextSymbolID()), members(members), onCall(onCall) {}

    Macro::Macro(
        std::function<Macro(FunctionArguments)> onCall
    ) : macro_id(getNextSymbolID()), members({}), onCall(onCall) {}

    Macro::Macro() : macro_id(getNextSymbolID()), onCall([](FunctionArguments){throw std::runtime_error("onCall is undefined"); return omtl::Macro{};}) {}
}; // namespace omtl