#include <estd/UnitTest.h>
#include <iostream>
#include <omtl/language.h>

int main() {
    UnitTests test;

    test.testBlock({
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

    test.testBlock({
        std::string inArgs;
        std::string outArgs;
        OmtlFunction fTest{
            "[x: Number, Number, y: String, z: [x: Number, y: String]]",
            [&](OmtlObject* args) { outArgs = args->toString(); },
        };

        cptr<OmtlObject> args = new OmtlObject{
            "Tuple",
            "arguments",
            {
                new OmtlNumericObject{"0", 0},
                new OmtlNumericObject{"x", 0},
                new OmtlStringObject{"y", "testString"},
                new OmtlObject{
                    "Tuple",
                    "z",
                    {
                        new OmtlNumericObject{"x", "11"},
                        new OmtlStringObject{"y", "testString2"},
                    },
                },
            },
        };
        inArgs = args->toString();

        fTest.call(args.get());

        // std::cout << fTest.argTemplate.toString() << std::endl;
        return inArgs == outArgs;
    });

    test.testBlock({
        cptr<OmtlObject> args = new OmtlObject{
            "Tuple",
            "arguments",
            {
                new OmtlNumericObject{"x", 0},
                new OmtlStringObject{"y", "testString"},
                new OmtlObject{
                    "Tuple",
                    "z",
                    {
                        new OmtlNumericObject{"x", "11"},
                        new OmtlStringObject{"y", "testString2"},
                    },
                },
            },
        };
        bool testFailed = false;
        OmtlFunction fTest{
            "[x: Number, y: String, z: [x: Number, y: String]]",
            [&](OmtlObject* args) {
                if (!args->publicMembers.count("x")) { testFailed = true; }
                if (!args->publicMembers.count("y")) { testFailed = true; }
                if (!args->publicMembers.count("z")) { testFailed = true; }
                if (!args->publicMembers.at("z")->publicMembers.count("x")) { testFailed = true; }
                if (!args->publicMembers.at("z")->publicMembers.count("y")) { testFailed = true; }
                if (args->publicMembers.at("z")->publicMembers.count("z")) { testFailed = true; }
            },
        };
        fTest.call(args.get());
        return !testFailed;
    });

    test.testBlock({
        cptr<OmtlObject> args = new OmtlObject{
            "Tuple",
            "arguments",
            {
                new OmtlNumericObject{"x", 0},
                new OmtlStringObject{"y", "testString"},
                new OmtlObject{
                    "Tuple",
                    "z",
                    {
                        new OmtlNumericObject{"x", "11"},
                    },
                },
            },
        };
        bool testFailed = false;
        OmtlFunction fTest{
            "[x: Number, y: String, z: [x: Number, y: String]]",
            [&](OmtlObject* args) { testFailed = true; },
        };
        try {
            fTest.call(args.get());
        } catch (std::runtime_error& e) { return estd::string_util::contains(e.what(), "mismatch", true); }
        return !testFailed;
    });

    std::cout << test.getStats() << std::endl;
    return 0;
}