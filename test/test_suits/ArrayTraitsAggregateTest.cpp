/**
 * @file ArrayTraitsAggregateTest.cpp
 * @author DuYicheng
 * @date 2025-07-24
 * @brief 数组特征的聚合类型测试
 */

#include "emdevif_test_framework.h"

import emdevif.util.ArrayTraitsAggregate;

TEST_SUIT(ArrayTraitsAggregateTest)
{
    TEST_CASE_BEGIN(ZeroTest);
    {
        struct A : public emdevif::ArrayTraitsAggregate<int, 0> {
        };

        A a;

        static_assert(sizeof(A) == sizeof(int));
        static_assert(sizeof a == sizeof(int));

        EXPECT_TRUE(a.at(0) == nullptr);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(OneTest);
    {
        struct A : public emdevif::ArrayTraitsAggregate<int, 1> {
            int a;
        };

        [[maybe_unused]] A a{.a = 114514};

        // 要加上 ArrayTraitsAggregate 中用于占位的那一个成员的大小
        static_assert(sizeof a == sizeof(int) + sizeof(int));
        INT_EXPECT_EQ(a.a, a[0])->MESSAGE("a.a = %d, a[0] = %d", a.a, a[0]);
        INT_EXPECT_EQ(a.a, 114514);

        a.a = 2;
        INT_EXPECT_EQ(a.a, a[0])->MESSAGE("a.a = %d, a[0] = %d", a.a, a[0]);
        INT_EXPECT_EQ(a.a, 2);

        a[0] = -123;
        INT_EXPECT_EQ(a.a, a[0])->MESSAGE("a.a = %d, a[0] = %d", a.a, a[0]);
        INT_EXPECT_EQ(a.a, -123);

        EXPECT_TRUE(a.at(1) == nullptr);
        EXPECT_TRUE(a.at(2) == nullptr);
        EXPECT_TRUE(a.at(-1) == nullptr);
        EXPECT_TRUE(a.at(0) != nullptr && *a.at(0) == -123);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(ThreeTest);
    {
        struct A : public emdevif::ArrayTraitsAggregate<int, 3> {
            int a, b, c;
        };

        [[maybe_unused]] A a{.a = 114514, .b = -143, .c = 1919810};

        static_assert(sizeof a == sizeof(int) * 3 + sizeof(int));
        INT_EXPECT_EQ(a.a, a[0])->MESSAGE("a.a = %d, a[0] = %d", a.a, a[0]);
        INT_EXPECT_EQ(a.a, 114514);
        INT_EXPECT_EQ(a.b, a[1])->MESSAGE("a.b = %d, b[1] = %d", a.b, a[0]);
        INT_EXPECT_EQ(a.b, -143);
        INT_EXPECT_EQ(a.c, a[2])->MESSAGE("a.c = %d, c[2] = %d", a.c, a[0]);
        INT_EXPECT_EQ(a.c, 1919810);

        a = {.a = 2, .b = -1, .c = 5};
        INT_EXPECT_EQ(a.a, a[0])->MESSAGE("a.a = %d, a[0] = %d", a.a, a[0]);
        INT_EXPECT_EQ(a.a, 2);
        INT_EXPECT_EQ(a.b, a[1])->MESSAGE("a.b = %d, b[1] = %d", a.b, a[0]);
        INT_EXPECT_EQ(a.b, -1);
        INT_EXPECT_EQ(a.c, a[2])->MESSAGE("a.c = %d, c[2] = %d", a.c, a[0]);
        INT_EXPECT_EQ(a.c, 5);

        a[0] = -123;
        a[1] = 456;
        a[2] = 2355;
        INT_EXPECT_EQ(a.a, a[0])->MESSAGE("a.a = %d, a[0] = %d", a.a, a[0]);
        INT_EXPECT_EQ(a.a, -123);
        INT_EXPECT_EQ(a.b, a[1])->MESSAGE("a.b = %d, b[1] = %d", a.b, a[0]);
        INT_EXPECT_EQ(a.b, 456);
        INT_EXPECT_EQ(a.c, a[2])->MESSAGE("a.c = %d, c[2] = %d", a.c, a[0]);
        INT_EXPECT_EQ(a.c, 2355);

        EXPECT_TRUE(a.at(3) == nullptr);
        EXPECT_TRUE(a.at(4) == nullptr);
        EXPECT_TRUE(a.at(-1) == nullptr);
        EXPECT_TRUE(a.at(0) != nullptr && *a.at(0) == -123);
        EXPECT_TRUE(a.at(1) != nullptr && *a.at(1) == 456);
        EXPECT_TRUE(a.at(2) != nullptr && *a.at(2) == 2355);
    }
    TEST_CASE_END();
}

void arrayTraitsAggregateTest()
{
    RUN_SUIT(ArrayTraitsAggregateTest);
}
