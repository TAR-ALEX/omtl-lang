#pragma once

#include <deque>
#include <estd/ptr.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <omtl/Object.hpp>
#include <omtl/ParseTree.hpp>
#include <omtl/Tokenizer.hpp>
#include <sstream>

namespace omtl {

    using namespace estd::shortnames;

    struct NumericObject : public Object {
        estd::BigDecimal data;

        // TODO: add all member functions related to numbers

        NumericObject(std::string name, estd::BigDecimal d = 0) : data{d} {
            this->name = name;
            this->type = "Number";
        }
        virtual std::string toString() { return type + "(" + name + "): " + data.toString(); }

        virtual std::string cppImplementStructure() {
            // FIXME: throw an error if this is called? This is a native construct and should already be declared
            return "int64_t " + name + " = 0;";
        }

        virtual std::string cppCreateInstance() {
            return "int64_t " + name + " = " + data.toString() + ";"; // TODO: init with actual value
        }

        virtual clonable* clone() const { return new NumericObject(*this); }
    };
}; // namespace omtl