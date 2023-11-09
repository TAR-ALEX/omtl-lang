#pragma once

#include <deque>
#include <estd/ptr.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <omtl/Object.hpp>
#include <omtl/ParseTree.hpp>
#include <omtl/Tokenizer.hpp>
#include <omtl/argumentMatching.h>
#include <sstream>

namespace omtl {

    using namespace estd::shortnames;

    struct StringObject : public Object {
        std::string data;

        // TODO: add all member functions related to strings.

        StringObject(std::string name, std::string data = "") : data{data} {
            this->name = name;
            this->type = "String";
        }
        virtual std::string toString() { return type + "(" + name + "): " + data; }

        virtual std::string cppImplementStructures() {
            // FIXME: throw an error if this is called? This is a native construct and should already be declared
            return "std::string " + name + " = \"\";";
        }

        virtual std::string cppCreateInstance() {
            return "std::string " + name + " = \"" + estd::string_util::escape_string(data) + "\";";
        }

        virtual clonable* clone() const { return new StringObject(*this); }
    };
}; // namespace omtl