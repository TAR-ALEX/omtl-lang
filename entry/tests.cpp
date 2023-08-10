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

        std::string target = "bmw{Car} [\n"
                             "  fuelTank{Reservoir} [\n"
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

    OmtlFunction fTest{
        "[x: Number, y: String]",
        [](OmtlObject args) { std::cout << args.toString(); },
    };

    std::cout << fTest.argTemplate.toString() << std::endl;

    test.testBool(true);
    std::cout << test.getStats() << std::endl;
    return 0;
}