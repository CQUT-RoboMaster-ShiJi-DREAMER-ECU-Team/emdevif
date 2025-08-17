/**
 * @file BitInt_test.cpp
 * @author DuYicheng
 * @date 2025-08-17
 * @brief BitInt 测试
 */

#include <numeric>
#include <cstdint>

#include "emdevif_test_framework.h"

import emdevif.util.BitInt;

static TEST_SUIT(BitIntTest)
{
    using emdevif::BitInt;

    TEST_CASE_BEGIN(Basic)
    {
        constexpr auto int8_max = std::numeric_limits<int8_t>::max();
        constexpr auto int8_min = std::numeric_limits<int8_t>::min();

        BitInt<8> a = int8_max;
        INT_EXPECT_EQ(a, int8_max);
        EXPECT_TRUE(static_cast<int>(a) == int8_max);

        a = int8_min;
        INT_EXPECT_EQ(a, int8_min);
        EXPECT_TRUE(static_cast<int8_t>(a) == int8_min);

        a = 0;
        INT_EXPECT_EQ(a, 0);
        EXPECT_TRUE(static_cast<int>(a) == 0);

        constexpr auto int16_max = std::numeric_limits<int16_t>::max();
        constexpr auto int16_min = std::numeric_limits<int16_t>::min();

        BitInt<16> b{int16_max};
        INT_EXPECT_EQ(b, int16_max);
        EXPECT_TRUE(static_cast<int>(b) == int16_max);

        b = int16_min;
        INT_EXPECT_EQ(b, int16_min);
        EXPECT_TRUE(static_cast<int>(b) == int16_min);

        b = 0;
        INT_EXPECT_EQ(b, 0);
        EXPECT_TRUE(static_cast<int16_t>(b) == 0);

        constexpr auto int32_max = std::numeric_limits<int32_t>::max();
        constexpr auto int32_min = std::numeric_limits<int32_t>::min();

        BitInt<32> c{int32_max};
        INT_EXPECT_EQ(c, int32_max);
        EXPECT_TRUE(static_cast<int32_t>(c) == int32_max);

        c = int32_min;
        INT_EXPECT_EQ(c, int32_min);
        EXPECT_TRUE(static_cast<int32_t>(c) == int32_min);

        c = 0;
        INT_EXPECT_EQ(c, 0);
        EXPECT_TRUE(static_cast<int32_t>(c) == 0);

        constexpr auto int64_max = std::numeric_limits<int64_t>::max();
        constexpr auto int64_min = std::numeric_limits<int64_t>::min();

        BitInt<64> d{int64_max};
        EXPECT_TRUE(static_cast<int64_t>(d) == int64_max);

        d = int64_min;
        EXPECT_TRUE(static_cast<int64_t>(d) == int64_min);

        d = 0;
        EXPECT_TRUE(static_cast<int64_t>(d) == 0);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(NotStdIntBits)
    {
        static_assert(BitInt<4>::max() == 7);
        static_assert(BitInt<4>::min() == -8);

        BitInt<4> a = 0b101;  // 5
        INT_EXPECT_EQ(a, 5);
        EXPECT_TRUE(static_cast<int8_t>(a) == 5);

        a = -0b101;  // -5
        INT_EXPECT_EQ(a, -5);
        EXPECT_TRUE(static_cast<int8_t>(a) == -5);

        a = a.max();
        INT_EXPECT_EQ(a, 7);
        EXPECT_TRUE(static_cast<int8_t>(a) == 7);

        a = a.min();
        INT_EXPECT_EQ(a, -8);
        EXPECT_TRUE(static_cast<int8_t>(a) == -8);

        const BitInt<6> c1 = 10;
        INT_EXPECT_EQ(c1, 10);
        BitInt<5> c2 = c1;
        INT_EXPECT_EQ(c2, 10);

        c2 = a;
        INT_EXPECT_EQ(-8, a);
        INT_EXPECT_EQ(c2, a);

        c2 += 5;
        INT_EXPECT_EQ(c2, -3);

        c2 += a;
        INT_EXPECT_EQ(c2, -11);

        int t = c1 + c2 + 1;
        INT_EXPECT_EQ(t, 0);

        t = 13 + c2;
        INT_EXPECT_EQ(t, 2);

        BitInt<6> c6;

        c6 = 0b11101;
        INT_EXPECT_EQ(c6 & 0b11111, 0b11101);
    }
    TEST_CASE_END();
}

void bitIntTest()
{
    RUN_SUIT(BitIntTest);
}
