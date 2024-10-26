#include <omtl/MacroParser.hpp>

namespace omtl {
    void MacroParser::parseGlobal(omtl::Element tree){
        for(auto& elem: tree){
            if(elem.second.isNumber()){
                globalScope.insert({elem.first, NumberLiteral(elem.second.getNumber())});
            }else if(elem.second.isString()){
                globalScope.insert({elem.first, StringLiteral(elem.second.getString())});
            }else if(elem.second.isName()){
                globalScope.insert({elem.first, globalScope.at(elem.second.getName())});
            }else if(elem.second.isStatement()){

            }
        }
    }
};