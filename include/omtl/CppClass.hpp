#pragma once

#include <deque>
#include <estd/ptr.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <omtl/Function.hpp>
#include <omtl/NumericObject.hpp>
#include <omtl/Object.hpp>
#include <omtl/ParseTree.hpp>
#include <omtl/StringObject.hpp>
#include <omtl/Tokenizer.hpp>
#include <omtl/Util.hpp>
#include <sstream>


// NOTE: none of these classes handle naming collisions. You must use them wisley.
namespace omtl {
    class CppObject;
    class CppClass {
    public:
        std::string name;

        AutonameMap<jptr<CppObject>> privateMembers; // private member variables and functions.
        AutonameMap<jptr<CppObject>> publicMembers;  // public variables and functions.

        CppClass(
            std::string name,
            AutonameMap<jptr<CppObject>> publicMembers = {},
            AutonameMap<jptr<CppObject>> privateMembers = {}
        ) :
            name(name),
            privateMembers(privateMembers), publicMembers(publicMembers) {}

        virtual ~CppClass() = default;
        virtual std::string cppDeclare();
        virtual std::string cppImplement();
    };

    class CppObject {
    public:
        jptr<CppClass> creator;
        std::string name;

        AutonameMap<jptr<CppObject>> privateMembers; // this is the actual instance.
        AutonameMap<jptr<CppObject>> publicMembers;  // this is the actual instance.

        virtual std::string cppDeclare(std::string terminator = ";");
        virtual std::string cppDeclarePtr(std::string terminator = ";");
        virtual std::string cppReference();
        virtual std::string cppReferencePtr();

        virtual jptr<CppObject> cppGetMember(std::string s);

        // virtual std::string cppConstruct() { return ""; }; // FIXME: make this init the class, decide if this is needed

        CppObject(jptr<CppClass> creator, std::string name);
        virtual ~CppObject() = default;
    };

    class CppFunction {
    public:
        std::string name;

        std::vector<jptr<CppObject>> input;  // private member variables and functions.
        std::vector<jptr<CppObject>> output; // public variables and functions.

        std::string body = "";

        virtual std::string cppDeclare();
        virtual std::string cppImplement();

        CppFunction(
            std::string name,
            std::vector<jptr<CppObject>> input = {},
            std::vector<jptr<CppObject>> output = {},
            std::string body = ""
        ) :
            name(name),
            input(input), output(output), body(body){};
        virtual ~CppFunction() = default;
    };


    // implementation

    // CppClass

    std::string CppClass::cppDeclare() { return "struct " + name + ";"; }

    std::string CppClass::cppImplement() {
        std::stringstream ss;

        for (auto& [k, v] : publicMembers) { ss << v->cppDeclare() << " " << std::endl; }
        for (auto& [k, v] : privateMembers) { ss << v->cppDeclare() << " " << std::endl; }

        return "class " + name + " {\n" + estd::string_util::indent(ss.str(), indentation) + "};";
    };

    // CppObject

    std::string CppObject::cppDeclare(std::string terminator) {
        std::stringstream ss;
        auto& type = creator->name;

        return type + " " + name + terminator; //TODO: initialize the individual values
    }

    inline std::string CppObject::cppDeclarePtr(std::string terminator) {
        std::stringstream ss;
        auto& type = creator->name;

        return type + " * " + name + terminator; //TODO: initialize the individual values
    }

    inline std::string CppObject::cppReference() { return name; }
    inline std::string CppObject::cppReferencePtr() { return "&(" + name + ")"; }

    inline jptr<CppObject> CppObject::cppGetMember(std::string s) { 
        try{
            return publicMembers.at(s);
        }catch(...){
            return privateMembers.at(s);
        }
    }

    inline CppObject::CppObject(jptr<CppClass> creator, std::string name) : creator(creator), name(name) {
        for (auto& [key, value] : creator->publicMembers) {
            publicMembers.insert(jptr<CppObject>(*value));
            publicMembers[value->name]->name = "(" + name + "." + value->name + ")";
        }
    };

    // CppFunction
    inline std::string omtl::CppFunction::cppDeclare() {
        std::stringstream ss;

        ss << "void " << name << "(";

        bool firstLoop = true;
        for (auto& v : input) {
            if (!firstLoop) ss << ", ";
            firstLoop = false;
            ss << v->cppDeclare("");
        }
        for (auto& v : output) {
            if (!firstLoop) ss << ", ";
            firstLoop = false;
            ss << v->cppDeclarePtr("");
        }
        ss << ");";
        return ss.str();
    }

    inline std::string CppFunction::cppImplement() {
        std::stringstream ss;
        std::string decl = cppDeclare();
        ss << decl.substr(0, decl.size() - 1);
        ss << "{\n" + estd::string_util::indent(body, indentation) + "\n};";
        return ss.str();
    }

}; // namespace omtl