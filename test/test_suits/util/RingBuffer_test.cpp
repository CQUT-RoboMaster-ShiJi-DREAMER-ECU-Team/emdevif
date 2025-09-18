/**
 * @file RingBuffer_test.cpp
 * @author DuYicheng
 * @date 2025-09-18
 * @brief 环形缓冲区测试
 */

#include "emdevif_test_framework.h"

#include <array>

import emdevif.util.ringBuffer;

TEST_SUIT(RingBufferTest)
{
    TEST_CASE_BEGIN(BasicTest)
    {
        emdevif::RingBuffer<char, 10> buffer;

        UINT_ASSERT_EQ(buffer.size(), 10, "");
        UINT_ASSERT_EQ(buffer.usedSlots(), 0, "");
        UINT_ASSERT_EQ(buffer.remainSlots(), 10, "");
        ASSERT_TRUE(buffer.isEmpty(), "");
        ASSERT_FALSE(buffer.isFull(), "");

        buffer.push('A');
        UINT_EXPECT_EQ(buffer.usedSlots(), 1);
        UINT_EXPECT_EQ(buffer.remainSlots(), 9);
        INT_EXPECT_EQ(buffer.peek(), 'A');
        INT_EXPECT_EQ(buffer.pop(), 'A');

        UINT_ASSERT_EQ(buffer.usedSlots(), 0, "");
        UINT_ASSERT_EQ(buffer.remainSlots(), 10, "");
        ASSERT_TRUE(buffer.isEmpty(), "");
        ASSERT_FALSE(buffer.isFull(), "");

        buffer.push('B');
        buffer.push('C');
        UINT_EXPECT_EQ(buffer.usedSlots(), 2);
        UINT_EXPECT_EQ(buffer.remainSlots(), 8);
        INT_EXPECT_EQ(buffer.peek(), 'B');
        INT_EXPECT_EQ(buffer.pop(), 'B');
        INT_EXPECT_EQ(buffer.peek(), 'C');
        INT_EXPECT_EQ(buffer.pop(), 'C');

        UINT_ASSERT_EQ(buffer.usedSlots(), 0, "");
        UINT_ASSERT_EQ(buffer.remainSlots(), 10, "");
        ASSERT_TRUE(buffer.isEmpty(), "");
        ASSERT_FALSE(buffer.isFull(), "");

        buffer.push('D');
        buffer.push('E');
        char out_peek[2];
        buffer.peek(out_peek);
        INT_EXPECT_EQ(out_peek[0], 'D');
        INT_EXPECT_EQ(out_peek[1], 'E');

        std::array<char, 2> out_pop;  // NOLINT(*-pro-type-member-init)
        buffer.pop(out_pop);
        INT_EXPECT_EQ(out_pop[0], 'D');
        INT_EXPECT_EQ(out_pop[1], 'E');

        UINT_ASSERT_EQ(buffer.usedSlots(), 0, "");
        UINT_ASSERT_EQ(buffer.remainSlots(), 10, "");
        ASSERT_TRUE(buffer.isEmpty(), "");
        ASSERT_FALSE(buffer.isFull(), "");

        constexpr char test_data1[] = {'F', 'G', 'H', 'I', 'J', 'K', 'L'};
        buffer.push(test_data1);
        UINT_EXPECT_EQ(buffer.usedSlots(), 7);
        UINT_EXPECT_EQ(buffer.remainSlots(), 3);

        char out_array1[std::size(test_data1)];
        buffer.pop(out_array1);
        EXPECT_TRUE(std::equal(std::begin(test_data1), std::end(test_data1), out_array1));
        UINT_ASSERT_EQ(buffer.usedSlots(), 0, "");
        UINT_ASSERT_EQ(buffer.remainSlots(), 10, "");
        ASSERT_TRUE(buffer.isEmpty(), "");
        ASSERT_FALSE(buffer.isFull(), "");

        constexpr char test_data_truncate[] = {'M', 'N', 'O', 'P', 'Q'};
        buffer.push(test_data_truncate);
        UINT_EXPECT_EQ(buffer.usedSlots(), 5);
        UINT_EXPECT_EQ(buffer.remainSlots(), 5);

        std::array<char, std::size(test_data_truncate)> out_array2;  // NOLINT(*-pro-type-member-init)
        buffer.pop(out_array2);
        EXPECT_TRUE(std::equal(std::begin(test_data_truncate), std::end(test_data_truncate), out_array2.data()));
        UINT_ASSERT_EQ(buffer.usedSlots(), 0, "");
        UINT_ASSERT_EQ(buffer.remainSlots(), 10, "");
        ASSERT_TRUE(buffer.isEmpty(), "");
        ASSERT_FALSE(buffer.isFull(), "");
    }
    TEST_CASE_END();
}

void ringBufferTest()
{
    RUN_SUIT(RingBufferTest);
}
