#include <estd/UnitTest.h>
#include <iostream>
#include <omtl/language.h>

int main() {
    UnitTests test;

    test.testLambda([]() {
        OmtlObject oTest{
            "Car",
            "bmw",
            {
                new OmtlNumericObject{"numwheels", "4"},
                new OmtlNumericObject{"numHorsepower", "250"},
                new OmtlStringObject{"modelName", "330ci"},
                new OmtlObject{
                    "Reservoir",
                    "fuelTank",
                    {
                        new OmtlNumericObject{"volumeLiters", "10"},
                        new OmtlStringObject{"fluidType", "gasoline"},
                    },
                },
            },
        };

        std::string target = "bmw{Car}: [\n"
                             "  fuelTank{Reservoir}: [\n"
                             "    fluidType{String}: gasoline\n"
                             "    volumeLiters{Number}: 10\n"
                             "  ]\n"
                             "  modelName{String}: 330ci\n"
                             "  numHorsepower{Number}: 250\n"
                             "  numwheels{Number}: 4\n"
                             "]";
        // std::cout << oTest.toString() << std::endl;
        return oTest.toString() == target;
    });

    test.testLambda([]() {
        std::string inArgs;
        std::string outArgs;
        OmtlFunction fTest{
            "[x: Number, y: String, z: [x: Number, y: String]]",
            [&](OmtlObject* args) { outArgs = args->toString(); },
        };

        cptr<OmtlObject> args = new OmtlObject{
            "Tuple",
            "arguments",
            {new OmtlNumericObject{"x", 0},
             new OmtlStringObject{"y", "testString"},
             OmtlObject{
                 "Tuple",
                 "z",
                 {
                     new OmtlNumericObject{"x", "11"},
                     new OmtlStringObject{"y", "testString2"},
                 },
             }},
        };
        inArgs = args->toString();

        fTest.call(args.get());

        // std::cout << fTest.argTemplate.toString() << std::endl;
        return inArgs == outArgs;
    });

    test.testLambda([]() {
        OmtlFunction fTest{
            "[x: Number, y: String, z: [x: Number, y: String]]",
            [&](OmtlObject* args) {},
        };

        cptr<OmtlObject> args = new OmtlObject{
            "Tuple",
            "arguments",
            {new OmtlNumericObject{"x", 0},
             new OmtlStringObject{"y", "testString"},
             OmtlObject{
                 "Tuple",
                 "z",
             }},
        };
        try {
            fTest.call(args.get());
        } catch (std::runtime_error& e) { return estd::string_util::contains(e.what(), "mismatch", true); }
        return false;
    });

    std::cout << test.getStats() << std::endl;
    return 0;
}