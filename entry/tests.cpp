#include <estd/UnitTest.h>
#include <iostream>
#include <omtl/Language.hpp>

int main() {
    using namespace omtl;
    UnitTests test;

    test.testBlock({
        CppClass intg{"int"};

        CppClass c{
            "Human",
            {
                CppObject{intg, "arms"},
                CppObject{intg, "legs"},
            }
        };

        auto instance = CppObject{c, "test"};

        std::string out = c.cppImplement();
        std::string target = "class Human {\n  int arms; \n  int legs; \n};";
        return out == target && instance.cppGetMember("arms")->cppReference() == "(test.arms)";
    });
    test.testBlock({
        CppClass intg{"int"};

        CppFunction c{
            "main",

            {CppObject{intg, "arg1"}, CppObject{intg, "arg2"}},
            {CppObject{intg, "ret1"}},

        };

        std::string out = c.cppDeclare();
        std::string target = "void main(int arg1, int arg2, int * ret1);";
        return out == target;
    });
    test.testBlock({
        CppClass intg{"int"};

        CppFunction c{
            "main",

            {CppObject{intg, "arg1"}, CppObject{intg, "arg2"}},
            {CppObject{intg, "ret1"}},
            "ret1 = arg1 + arg2;\nret1 = ret1 * arg2;"
        };

        std::string out = c.cppImplement();
        std::string target = "void main(int arg1, int arg2, int * ret1){\n  ret1 = arg1 + arg2;\n  ret1 = ret1 * arg2;\n};";
        return out == target;
    });
    test.testBlock({
        Object oTest{
            "Car",
            "bmw",
            {
                new NumericObject{"numwheels", "4"},
                new NumericObject{"numHorsepower", "250"},
                new StringObject{"modelName", "330ci"},
                new Object{
                    "Reservoir",
                    "fuelTank",
                    {
                        new NumericObject{"volumeLiters", "10"},
                        new StringObject{"fluidType", "gasoline"},
                    },
                },
            },
        };

        std::string target = "Car(bmw): [\n"
                             "  Reservoir(fuelTank): [\n"
                             "    String(fluidType): gasoline, \n"
                             "    Number(volumeLiters): 10, \n"
                             "  ], \n"
                             "  String(modelName): 330ci, \n"
                             "  Number(numHorsepower): 250, \n"
                             "  Number(numwheels): 4, \n"
                             "]";
        return oTest.toString() == target;
    });

    test.testBlock({
        Object oTest{
            "Car",
            "bmw",
            {
                new NumericObject{"numwheels", "4"},
                new NumericObject{"numHorsepower", "250"},
                new StringObject{"modelName", "330ci"},
                new Object{
                    "Reservoir",
                    "fuelTank",
                    {
                        new NumericObject{"volumeLiters", "10"},
                        new StringObject{"fluidType", "gasoline"},
                    },
                },
            },
        };

        std::string target = "Car(bmw): [\n"
                             "  Reservoir(fuelTank): [\n"
                             "    String(fluidType): gasoline, \n"
                             "    Number(volumeLiters): 10, \n"
                             "  ], \n"
                             "  String(modelName): 330ci, \n"
                             "  Number(numHorsepower): 250, \n"
                             "  Number(numwheels): 4, \n"
                             "]";
        //std::cout << oTest.toString() << std::endl;
        return oTest.toString() == target;
    });

    test.testBlock({
        std::string inArgs;
        std::string outArgs;
        Function fTest{
            "[x: Number, Number, y: String, z: [x: Number, y: String]]",
            [&](Object* args) { outArgs = args->toString(); },
        };

        cptr<Object> args = new Object{
            "Tuple",
            "arguments",
            {
                new NumericObject{"0", 0},
                new NumericObject{"x", 0},
                new StringObject{"y", "testString"},
                new Object{
                    "Tuple",
                    "z",
                    {
                        new NumericObject{"x", "11"},
                        new StringObject{"y", "testString2"},
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
        cptr<Object> args = new Object{
            "Tuple",
            "arguments",
            {
                new NumericObject{"x", 0},
                new StringObject{"y", "testString"},
                new Object{
                    "Tuple",
                    "z",
                    {
                        new NumericObject{"x", "11"},
                        new StringObject{"y", "testString2"},
                    },
                },
            },
        };
        bool testFailed = false;
        Function fTest{
            "[x: Number, y: String, z: [x: Number, y: String]]",
            [&](Object* args) {
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
        cptr<Object> args = new Object{
            "Tuple",
            "arguments",
            {
                new NumericObject{"x", 0},
                new StringObject{"y", "testString"},
                new Object{
                    "Tuple",
                    "z",
                    {
                        new NumericObject{"x", "11"},
                    },
                },
            },
        };
        bool testFailed = false;
        Function fTest{
            "[x: Number, y: String, z: [x: Number, y: String]]",
            [&](Object* args) { testFailed = true; },
        };
        try {
            fTest.call(args.get());
        } catch (std::runtime_error& e) { return estd::string_util::contains(e.what(), "mismatch", true); }
        return !testFailed;
    });

    std::cout << test.getStats() << std::endl;
    return 0;
}