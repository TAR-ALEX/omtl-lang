#pragma once

#include <omtl/Symbol.hpp>
#include <omtl/MacroTypes.hpp>
#include <omtl/ParseTree.hpp>
#include <vector>

namespace omtl {
    class MacroParser{
    public:
        std::map<std::string, omtl::Object> globalScope;
        void parseGlobal(omtl::Element tree);
    };
};