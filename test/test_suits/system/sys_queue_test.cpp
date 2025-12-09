/**
 * @file sys_queue_test.cpp
 * @brief 由操作系统提供的消息队列测试
 */

#include "emdevif_test_framework.h"

#include "heap_usage_checker.hpp"

#include <type_traits>

import emdevif.container.messageQueue;
import emdevif.sys.sysQueue;

TEST_SUIT(SysQueueBasicTest)
{
    RECORD_MEMORY_INFO();

    TEST_CASE_BEGIN(ConstructDirectly)
    {
        emdevif::MessageQueue<emdevif::SysQueue, int, 5> queue =
            emdevif::SysQueue<int, 5>::create({.name = "BasicTestQueue"});
        static_assert(std::is_same_v<decltype(queue)::ImplType, emdevif::SysQueue<int, 5>>);

        UINT_EXPECT_EQ(queue.remainCount(), 5);
        UINT_EXPECT_EQ(queue.storeCount(), 0);

        int ret;
        EXPECT_TRUE(queue.pop(false) == emdevif::ErrorCode::Empty);
        EXPECT_TRUE(queue.peek(false, ret) == emdevif::ErrorCode::Empty);

        EXPECT_TRUE(queue.push(false, 5) == emdevif::ErrorCode::Success);
        EXPECT_TRUE(queue.push(false, -345) == emdevif::ErrorCode::Success);
        EXPECT_TRUE(queue.push(false, 114514) == emdevif::ErrorCode::Success);

        UINT_EXPECT_EQ(queue.remainCount(), 2);
        UINT_EXPECT_EQ(queue.storeCount(), 3);

        EXPECT_TRUE(queue.pop(false, ret) == emdevif::ErrorCode::Success);
        INT_EXPECT_EQ(ret, 5);
        EXPECT_TRUE(queue.pop(false, ret) == emdevif::ErrorCode::Success);
        INT_EXPECT_EQ(ret, -345);
        EXPECT_TRUE(queue.pop(false, ret) == emdevif::ErrorCode::Success);
        INT_EXPECT_EQ(ret, 114514);

        UINT_EXPECT_EQ(queue.remainCount(), 5);
        UINT_EXPECT_EQ(queue.storeCount(), 0);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(FactoryConstruct)
    {
        emdevif::MessageQueue<emdevif::SysQueue, int, 5> queue;
        static_assert(std::is_same_v<decltype(queue)::ImplType, emdevif::SysQueue<int, 5>>);

        queue = decltype(queue)::ImplType::create({.name = "BasicTestQueue"});

        UINT_EXPECT_EQ(queue.remainCount(), 5);
        UINT_EXPECT_EQ(queue.storeCount(), 0);

        int ret;
        EXPECT_TRUE(queue.pop(false) == emdevif::ErrorCode::Empty);
        EXPECT_TRUE(queue.peek(false, ret) == emdevif::ErrorCode::Empty);

        EXPECT_TRUE(queue.push(false, 5) == emdevif::ErrorCode::Success);
        EXPECT_TRUE(queue.push(false, -345) == emdevif::ErrorCode::Success);
        EXPECT_TRUE(queue.push(false, 114514) == emdevif::ErrorCode::Success);

        UINT_EXPECT_EQ(queue.remainCount(), 2);
        UINT_EXPECT_EQ(queue.storeCount(), 3);

        EXPECT_TRUE(queue.pop(false, ret) == emdevif::ErrorCode::Success);
        INT_EXPECT_EQ(ret, 5);
        EXPECT_TRUE(queue.pop(false, ret) == emdevif::ErrorCode::Success);
        INT_EXPECT_EQ(ret, -345);
        EXPECT_TRUE(queue.pop(false, ret) == emdevif::ErrorCode::Success);
        INT_EXPECT_EQ(ret, 114514);

        UINT_EXPECT_EQ(queue.remainCount(), 5);
        UINT_EXPECT_EQ(queue.storeCount(), 0);
    }
    TEST_CASE_END();

    CHECK_MEMORY_LEAK();
}

void sysQueueTest()
{
    RUN_SUIT(SysQueueBasicTest);
}
