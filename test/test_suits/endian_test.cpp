/**
 * @file endian_test.cpp
 * @brief 端序测试
 */

#include <cstdint>

#include <bit>
#include <array>
#include <algorithm>

#include "emdevif_test_framework.h"

import emdevif.core.endian;

TEST_SUIT(EndianTest)
{
    TEST_CASE_BEGIN(byteSwapTest)
    {
        constexpr uint8_t u8 = 0x13U;
        UINT_EXPECT_EQ(emdevif::byteSwap(u8), u8);
        static_assert(emdevif::byteSwap(u8) == u8);

        constexpr uint16_t u16 = 0x1234U;
        UINT_EXPECT_EQ(emdevif::byteSwap(u16), 0x3412U);
        static_assert(emdevif::byteSwap(u16) == 0x3412U);

        constexpr uint32_t u32 = 0x12345678U;
        UINT_EXPECT_EQ(emdevif::byteSwap(u32), 0x78563412U);
        static_assert(emdevif::byteSwap(u32) == 0x78563412U);

        constexpr uint64_t u64 = 0x123456789ABCDEF0ULL;
        constexpr auto u64_swap = emdevif::byteSwap(u64);
        EXPECT_TRUE(u64_swap == 0xF0DEBC9A78563412ULL)->MESSAGE("u64 = %#Xll, u64_swap = %#Xll", u64, u64_swap);

        [EMDEVIF_TEST_LAMBDA_CAPTURE] {
            constexpr std::array<std::uint8_t, 4> arr{0x01, 0x23, 0x45, 0x67};
            auto arr_swap_expected = arr;
            std::ranges::reverse(arr_swap_expected);

            EXPECT_TRUE(emdevif::byteSwap(arr) == arr_swap_expected);
        }();
    }
    TEST_CASE_END();
}

void endianTest()
{
    RUN_SUIT(EndianTest);
}
