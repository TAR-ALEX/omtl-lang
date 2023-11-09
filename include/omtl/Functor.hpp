#pragma once

#include <omtl/Function.hpp>

namespace omtl {

    using namespace estd::shortnames;

    struct Functor : Object {
        Functor(std::string name, std::set<Function> funcs) {}
    };

}; // namespace omtl