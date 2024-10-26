#include <estd/UnitTest.h>
#include <iostream>
#include <omtl/Language.hpp>

int main() {
    using namespace omtl;
    UnitTests test;

    test.testBlock({
        omtl::Object obj1;
        omtl::Object obj2;
        
        return true;
    });

    // test.testBlock({
    //     omtl::Object in1;
    //     omtl::Object in2;
    //     omtl::FunctionSignature sig{"1", "2"};
    //     omtl::FunctionArguments args{{"1", in1}, {"2", in2}};
    //     bool actionDone = false;
    //     omtl::Function f{{sig}, [&](auto args) {
    //                          actionDone = true;
    //                          return omtl::Object{};
    //                      }};
    //     auto o = f.call(args);
    //     return actionDone;
    // });

    // test.testBlock({
    //     omtl::Object in1;
    //     omtl::Object in2;
    //     omtl::FunctionSignature sig{"1", "2"};
    //     omtl::FunctionArguments args{{"1", in1}};
    //     omtl::Function f{{sig}, [](auto args) { return omtl::Object{}; }};
    //     omtl::Object functor{"", "", {}, {f}};
    //     return !functor.matchesSignature(args);
    // });

    // test.testBlock({
    //     omtl::Object in1;
    //     omtl::Object in2;
    //     omtl::FunctionSignature sig{"1", "2"};
    //     omtl::FunctionArguments args{{"1", in1}, {"2", in2}};
    //     bool actionDone = false;
    //     omtl::Function f{{sig}, [&](auto args) {
    //                          actionDone = true;
    //                          return omtl::Object{};
    //                      }};

    //     omtl::Object functor{"", "", {}, {f}};
    //     auto o = functor.call(args);
    //     return actionDone;
    // });

    // test.testBlock({
    //     bool actionDone = false;

    //     omtl::Object functor{
    //         "",
    //         "",
    //         {},
    //         {
    //             omtl::Function{
    //                 {"1"},
    //                 [&](auto args) {
    //                     actionDone = true;
    //                     return omtl::Object{};
    //                 }
    //             },
    //             omtl::Function{
    //                 {"1", "2"},
    //                 [&](auto args) {
    //                     return omtl::Object{};
    //                 }
    //             },
    //         }
    //     };
    //     auto o = functor.call(omtl::FunctionArguments{{"1", omtl::Object{}},{"2", omtl::Object{}}});
    //     return !actionDone;
    // });

    // test.testBlock({
    //     bool actionDone = false;

    //     omtl::Object functor{
    //         "",
    //         "",
    //         {},
    //         {
    //             omtl::Function{
    //                 {"1"},
    //                 [&](auto args) {
    //                     actionDone = true;
    //                     return omtl::Object{};
    //                 }
    //             },
    //             omtl::Function{
    //                 {"1", "2"},
    //                 [&](auto args) {
    //                     return omtl::Object{};
    //                 }
    //             },
    //         }
    //     };
    //     auto o = functor.call(omtl::FunctionArguments{{"1", omtl::Object{}}});
    //     return actionDone;
    // });


    std::cout << test.getStats() << std::endl;
    return 0;
}