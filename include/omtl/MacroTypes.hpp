#pragma once

#include <omtl/Language.hpp>
#include <omtl/MacroTypes.hpp>
#include <vector>

namespace omtl {
    extern std::map<std::string, std::string> stringLiteralMap;
    extern std::map<std::string, estd::BigDecimal> numericLiteralMap;
    omtl::Macro StringLiteral(std::string s);

    omtl::Macro NumberLiteral(estd::BigDecimal num);
    omtl::Macro NullMacro();
}; // namespace omtl