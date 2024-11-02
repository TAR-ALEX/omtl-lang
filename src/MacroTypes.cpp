#include <omtl/MacroTypes.hpp>


namespace omtl {
    std::map<std::string, std::string>
        stringLiteralMap; // http://www.boost.org/doc/libs/1_63_0/libs/bimap/doc/html/index.html
    std::map<std::string, estd::BigDecimal>
        numericLiteralMap; // http://www.boost.org/doc/libs/1_63_0/libs/bimap/doc/html/index.html

    omtl::Macro StringLiteral(std::string s) {
        auto obj = omtl::Macro{};
        stringLiteralMap.emplace(obj.macro_id, s); // dedupe here
        obj.members.emplace(
            "+", omtl::Macro{[obj](omtl::FunctionArguments args) {
                // we can add non literal variables here, maybe even numbers, etc
                auto str1 = stringLiteralMap[obj.macro_id];
                auto str2 = stringLiteralMap[args.at(0).second.macro_id];
                return StringLiteral(str1 + str2);
            }} //
        );
        return obj;
    }

    omtl::Macro NumberLiteral(estd::BigDecimal num) {
        auto obj = omtl::Macro{};
        numericLiteralMap.emplace(obj.macro_id, num); // dedupe here
        obj.members.emplace(
            "+", omtl::Macro{[obj](omtl::FunctionArguments args) {
                // we can add non literal variables here, maybe even numbers, etc
                auto num1 = numericLiteralMap[obj.macro_id];
                auto num2 = numericLiteralMap[args.at(0).second.macro_id];
                return NumberLiteral(num1 + num2);
            }} //
        );
        return obj;
    }

    omtl::Macro NullMacro(){
        return omtl::Macro{};
    }

    void initGlobalScope() {
        //number define
        // globalScope.push_back(omtl::Macro{
        //     "String",
        //     "c_String",
        //     {},
        //     {
        //         omtl::Function{
        //             {"0"},
        //             [](auto args) {
        //                 std::cout << "Created String\n";
        //                 return omtl::Macro{
        //                     "",
        //                     "i_String",
        //                     {},
        //                     {},
        //                 };
        //             },
        //         },
        //     },
        // });
    }
}; // namespace omtl