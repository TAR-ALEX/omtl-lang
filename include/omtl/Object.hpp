#pragma once

#include <deque>
#include <estd/ptr.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <omtl/AutonameMap.hpp>
#include <omtl/ParseTree.hpp>
#include <omtl/Tokenizer.hpp>
#include <sstream>
#include <string>

namespace omtl {
    const std::string indentation = "  ";

    using namespace estd::shortnames;
    // // Some declarations for future use.
    // struct Object;
    // struct Function;

    // omtl object is an object or an overloadable function like object
    struct Object : estd::clonable {
        std::string name; // internal var name, should not be changed after adding it to an AutonameMap
        // The only true reason we need a name here is for debug purposes. The name used in the map is what really matters.

        std::string type; // name of class that created this. aka the type

        //FIXME: investigate if the parent can be removed from here and only kept for the functor.
        rptr<Object> parent; // useful for functions that need access to parent (instance) vars.
        // aka, object to the left using the dot syntax in C++ like objects. So far, only functors should use this.
        // could be null

        AutonameMap<cptr<Object>> privateMembers; // private member variables and functions.
        AutonameMap<cptr<Object>> publicMembers;  // public variables and functions.

        // Objects can take arguments. in tuples or by passing a single object to its right.
        // Without the tuple syntax there can be some ambiguity. (a possible solution is to not allow parameters without tuples if there are member variables/functions)
        // This basically makes the functors a special case.
        inline Object() : name(""), type("Null") {}
        inline Object(
            std::string type,
            std::string name,
            AutonameMap<cptr<Object>> publicMembers = {},
            AutonameMap<cptr<Object>> privateMembers = {}
        ) :
            name(name),
            type(type), privateMembers(privateMembers), publicMembers(publicMembers) {}

        virtual std::tuple<std::string, cptr<Object>> cppCall(cptr<Object> passed) {
            // error cannot call this object
            //FIXME: add return type
            return {"", new Object()};
        }


        virtual clonable* clone() const { return new Object(*this); }

        virtual std::string toString() {
            std::stringstream ss;

            for (auto& [k, v] : publicMembers) { ss << v->toString() << ", " << std::endl; }
            for (auto& [k, v] : privateMembers) { ss << v->toString() << ", " << std::endl; }
            return type + "(" + name + "): [\n" + estd::string_util::indent(ss.str(), indentation) + "]";
        }

        virtual std::string cppDeclareStructures() {
            return "struct " + type + ";";
        }

        virtual std::string cppImplementStructure() {
            std::stringstream ss;

            for (auto& [k, v] : publicMembers) { ss << v->cppCreateInstance() << " " << std::endl; }
            for (auto& [k, v] : privateMembers) { ss << v->cppCreateInstance() << " " << std::endl; }

            return "struct " + type + " {\n" + estd::string_util::indent(ss.str(), indentation) + "};";
        }

        virtual std::string cppDeclareMethods() {
            throw std::runtime_error("//FIXME: will be implemented later");
        }

        virtual std::string cppImplementMethods() {
            throw std::runtime_error("//FIXME: will be implemented later");
        }

        virtual std::string cppCreateInstance() {
            std::stringstream ss;

            return type + " " + name + ";"; //TODO: initialize the individual values
        }
    };

    // TODO: investigate is this is needed
    // not sure if this is needed yet, can use the generic object with the type populated as Null
    struct NullObject : public Object {
        NullObject() { type = "Null"; }
        virtual std::string toString() { return "Null"; }

        virtual clonable* clone() const { return new NullObject(*this); }
    };
}; // namespace omtl