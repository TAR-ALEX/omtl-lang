#pragma once

#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>

#include <estd/ptr.hpp>

#include <omtl/ParseTree.hpp>
#include <omtl/Tokenizer.hpp>

#include <omtl/Util.hpp>

#include <omtl/Function.hpp>
#include <omtl/NumericObject.hpp>
#include <omtl/Object.hpp>
#include <omtl/StringObject.hpp>

namespace omtl {
    // a class will generate objects when it is invoked
    class Class : public omtl::Object {
    public:
        cptr<omtl::Object> instance; // the instance that the class will create every time it is constructed

        virtual std::tuple<std::string, cptr<Object>> cppCall(cptr<Object> passed) {
            cptr<omtl::Object> instance;
            // error cannot call this object
            //FIXME: add return type
            return {"", instance}; // clones the instance
        }

        virtual clonable* clone() const { return new Class(*this); }

        virtual std::string cppDeclareStructures() { return "struct " + type + ";"; }

        virtual std::string cppImplementStructure() {
            std::stringstream ss;

            for (auto& [k, v] : publicMembers) { ss << v->cppCreateInstance() << " " << std::endl; }
            for (auto& [k, v] : privateMembers) { ss << v->cppCreateInstance() << " " << std::endl; }

            return "struct " + type + " {\n" + estd::string_util::indent(ss.str(), indentation) + "};";
        }

        virtual std::string cppDeclareMethods() { throw std::runtime_error("//FIXME: will be implemented later"); }

        virtual std::string cppImplementMethods() { throw std::runtime_error("//FIXME: will be implemented later"); }

        virtual std::string cppCreateInstance() {
            std::stringstream ss;

            return type + " " + name + ";"; //TODO: initialize the individual values
        }
    };
}; // namespace omtl