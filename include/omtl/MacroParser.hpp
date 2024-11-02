#pragma once

#include <omtl/Symbol.hpp>
#include <omtl/MacroTypes.hpp>
#include <omtl/ParseTree.hpp>
#include <vector>

namespace omtl {
    typedef std::map<std::string, omtl::Macro> ScopeType;
    class MacroParser{
    private:
        omtl::Macro parseSingleElement(omtl::Element e, ScopeType& scope);
    public:
        ScopeType globalScope;
        void parseGlobal(omtl::Element tree);
        omtl::Macro parseStatement(omtl::Element statement, ScopeType& scope);
        void parseTuple(omtl::Element tuple, ScopeType& scope);
    };
};