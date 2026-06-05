/**
 * @file When_test.cpp
 * @brief when 表达式测试
 */

#include <string_view>
#include <string>

#include "emdevif_test_framework.h"

#ifdef EMDEVIF_USE_MODULES
import emdevif.core.utils.when;
#else
    #include "emdevif/core/utils/when.hpp"
#endif

namespace {

int globalSideEffect = 0;

void resetGlobalSideEffect()
{
    globalSideEffect = 0;
}

TEST_SUIT(WhenBasicTest)
{
    TEST_CASE_BEGIN(IntMatch)
    {
        // clang-format off
        const auto result = emdevif::when(2,
                                          1, [] { return 10; },
                                          2, [] { return 20; },
                                          3, [] { return 30; });
        // clang-format on
        INT_EXPECT_EQ(result, 20);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(FirstMatchWins)
    {
        // clang-format off
        const auto result = emdevif::when(1,
                                          1, [] { return "first"; },
                                          1, [] { return "second"; });
        // clang-format on
        EXPECT_TRUE(std::string_view(result) == "first");
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(DefaultTag)
    {
        // clang-format off
        const auto result = emdevif::when(99,
                                          1, [] { return 10; },
                                          2, [] { return 20; },
                                          emdevif::default_tag, [] { return -1; });
        // clang-format on
        INT_EXPECT_EQ(result, -1);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(OnlyDefault)
    {
        // clang-format off
        const auto result = emdevif::when(0,
                                          emdevif::default_tag, [] { return 42; });
        // clang-format on
        INT_EXPECT_EQ(result, 42);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(ImplictDefault)
    {
        // clang-format off
        const auto result = emdevif::when(0,
                                          1, [] { return 42; });
        // clang-format on
        INT_EXPECT_EQ(int{}, 0);
        INT_EXPECT_EQ(result, 0);
    }
    TEST_CASE_END();
}

TEST_SUIT(WhenStringTest)
{
    TEST_CASE_BEGIN(CStringPointer)
    {
        const char* hello = "hello";
        const char* world = "world";
        // clang-format off
        const auto result = emdevif::when(hello,
                                          hello, [] { return 1; },
                                          world, [] { return 2; });
        // clang-format on
        INT_EXPECT_EQ(result, 1);
    }
    TEST_CASE_END();
}

enum class TestColor {
    Red,
    Green,
    Blue
};

TEST_SUIT(WhenEnumTest)
{
    TEST_CASE_BEGIN(EnumMatch)
    {
        // clang-format off
        const auto result = emdevif::when(TestColor::Green,
                                          TestColor::Red, [] { return 1; },
                                          TestColor::Green, [] { return 2; },
                                          TestColor::Blue, [] { return 3; });
        // clang-format on
        INT_EXPECT_EQ(result, 2);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(EnumDefault)
    {
        // clang-format off
        const auto result = emdevif::when(TestColor::Blue,
                                          TestColor::Red, [] { return 10; },
                                          TestColor::Green, [] { return 20; },
                                          emdevif::default_tag, [] { return 30; });
        // clang-format on
        INT_EXPECT_EQ(result, 30);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(EnumImplictDefault)
    {
        // clang-format off
        const auto result = emdevif::when(TestColor::Blue,
                                          TestColor::Red, [] { return 10; },
                                          TestColor::Green, [] { return 20; });
        // clang-format on
        INT_EXPECT_EQ(int{}, 0);
        INT_EXPECT_EQ(result, 0);
    }
    TEST_CASE_END();
}

TEST_SUIT(WhenSideEffectTest)
{
    TEST_CASE_BEGIN(OnlyMatchingBranchExecutes)
    {
        resetGlobalSideEffect();

        // clang-format off
        const auto result = emdevif::when(2,
                                          1, [] {
                                              globalSideEffect = 1;
                                              return 1;
                                          },
                                          2, [] {
                                              globalSideEffect = 2;
                                              return 2;
                                          },
                                          3, [] {
                                              globalSideEffect = 3;
                                              return 3;
                                          });
        // clang-format on
        INT_EXPECT_EQ(result, 2);
        INT_EXPECT_EQ(globalSideEffect, 2);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(DefaultBranchExecutesWhenNoMatch)
    {
        resetGlobalSideEffect();

        // clang-format off
        const auto result = emdevif::when(99,
                                          1, [] {
                                              globalSideEffect = 1;
                                              return 1;
                                          },
                                          emdevif::default_tag, [] {
                                              globalSideEffect = 999;
                                              return 999;
                                          });
        // clang-format on
        INT_EXPECT_EQ(result, 999);
        INT_EXPECT_EQ(globalSideEffect, 999);
    }
    TEST_CASE_END();
}

TEST_SUIT(WhenReturnTypeTest)
{
    TEST_CASE_BEGIN(FloatReturn)
    {
        // clang-format off
        const auto result = emdevif::when(1,
                                          1, [] { return 3.14f; },
                                          emdevif::default_tag, [] { return 0.0f; });
        // clang-format on
        FP_EXPECT_EQ(result, 3.14f);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(BoolReturn)
    {
        // clang-format off
        const auto result = emdevif::when(0,
                                          0, [] { return false; },
                                          1, [] { return true; });
        // clang-format on
        EXPECT_FALSE(result);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(VoidReturn)
    {
        resetGlobalSideEffect();

        // clang-format off
        emdevif::when(2,
            1, [] { globalSideEffect = 1; },
            2, [] { globalSideEffect = 2; },
            3, [] { globalSideEffect = 3; }
        );
        // clang-format on
        INT_EXPECT_EQ(globalSideEffect, 2);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(VoidReturnImplictDefault)
    {
        resetGlobalSideEffect();

        // clang-format off
        emdevif::when(-2,
            1, [] { globalSideEffect = 1; },
            2, [] { globalSideEffect = 2; },
            3, [] { globalSideEffect = 3; }
        );
        // clang-format on
        INT_EXPECT_EQ(globalSideEffect, 0);
    }
    TEST_CASE_END();
}

}  // namespace

void whenTest()
{
    RUN_SUIT(WhenBasicTest);
    RUN_SUIT(WhenStringTest);
    RUN_SUIT(WhenEnumTest);
    RUN_SUIT(WhenSideEffectTest);
    RUN_SUIT(WhenReturnTypeTest);
}
