/**
 * @file sys_message_slot_test.cpp
 * @brief 系统消息槽测试
 */

#include "emdevif_test_framework.h"

#include "heap_usage_checker.hpp"

#include <type_traits>

import emdevif.container.messageSlot;
import emdevif.sys.sysMessageSlot;
import emdevif.errorHandler;

TEST_SUIT(SysMessageSlotBasicTest)
{
    RECORD_MEMORY_INFO();

    auto testImplWithoutCreate = []<typename T>(emdevif_test_TestCaseContex& contex, emdevif::SysMessageSlot<T>& slot) {
        RUN_TEST_CASE_WITHIN_CONTEX_BEGIN(&contex)
        {
            static_assert(slot.maxItemCount() == 1);
            UINT_EXPECT_EQ(slot.maxItemCount(), 1);

            UINT_EXPECT_EQ(1, slot.remainCount());
            UINT_EXPECT_EQ(0, slot.storeCount());

            int ret;
            EXPECT_TRUE(slot.peek(false, ret) == emdevif::ErrorCode::Empty);

            EXPECT_TRUE(slot.forcePush(false, 114514) == emdevif::ErrorCode::Success);
            UINT_EXPECT_EQ(0, slot.remainCount());
            UINT_EXPECT_EQ(1, slot.storeCount());

            EXPECT_TRUE(slot.peek(false, ret) == emdevif::ErrorCode::Success);
            UINT_EXPECT_EQ(0, slot.remainCount());
            UINT_EXPECT_EQ(1, slot.storeCount());
            INT_EXPECT_EQ(ret, 114514);

            EXPECT_TRUE(slot.peek(false, ret) == emdevif::ErrorCode::Success);
            UINT_EXPECT_EQ(0, slot.remainCount());
            UINT_EXPECT_EQ(1, slot.storeCount());
            INT_EXPECT_EQ(ret, 114514);

            EXPECT_TRUE(slot.forcePush(false, -1919810) == emdevif::ErrorCode::Success);
            UINT_EXPECT_EQ(0, slot.remainCount());
            UINT_EXPECT_EQ(1, slot.storeCount());

            EXPECT_TRUE(slot.peek(false, ret) == emdevif::ErrorCode::Success);
            UINT_EXPECT_EQ(0, slot.remainCount());
            UINT_EXPECT_EQ(1, slot.storeCount());
            INT_EXPECT_EQ(ret, -1919810);

            EXPECT_TRUE(slot.peek(false, ret) == emdevif::ErrorCode::Success);
            UINT_EXPECT_EQ(0, slot.remainCount());
            UINT_EXPECT_EQ(1, slot.storeCount());
            INT_EXPECT_EQ(ret, -1919810);

            slot.clear();
            UINT_EXPECT_EQ(1, slot.remainCount());
            UINT_EXPECT_EQ(0, slot.storeCount());
            EXPECT_TRUE(slot.peek(false, ret) == emdevif::ErrorCode::Empty);
        }
        RUN_TEST_CASE_WITHIN_CONTEX_END();
    };

    TEST_CASE_BEGIN(ConstructDirectly)
    {
        emdevif::SysMessageSlot<int> slot = emdevif::SysMessageSlot<int>::create({.name = "BasicTestSlot"});
        static_assert(std::is_same_v<decltype(slot), emdevif::SysMessageSlot<int>>);

        emdevif_test_TestCaseContex contex = GET_THIS_TEST_CASE_CONTEXT();
        testImplWithoutCreate(contex, slot);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(FactoryConstruct)
    {
        emdevif::SysMessageSlot<int> slot;
        static_assert(std::is_same_v<decltype(slot), emdevif::SysMessageSlot<int>>);

        slot = decltype(slot)::create({.name = "BasicTestSlot"});

        emdevif_test_TestCaseContex contex = GET_THIS_TEST_CASE_CONTEXT();
        testImplWithoutCreate(contex, slot);
    }
    TEST_CASE_END();

    CHECK_MEMORY_LEAK();
}

void sysMessageSlotTest()
{
    RUN_SUIT(SysMessageSlotBasicTest);
}
