#pragma once

#include <omtl/Language.hpp>
#include <omtl/MacroTypes.hpp>
#include <vector>

namespace omtl {
    extern std::map<std::string, std::string> stringLiteralMap;
    extern std::map<std::string, estd::BigDecimal> numericLiteralMap;
    omtl::Object StringLiteral(std::string s);

    omtl::Object NumberLiteral(estd::BigDecimal num);
}; // namespace omtl