/**
 * @file sys_message_slot_test.cpp
 * @brief 系统消息槽测试
 */

#include "emdevif_test_framework.h"

#include "heap_usage_checker.hpp"

#include <type_traits>
#include <concepts>

#ifdef EMDEVIF_USE_MODULES
import emdevif.core.data_container.message_queue;
import emdevif.system.sys_message_slot;
import emdevif.core.error_handler;
#else
    #include "emdevif/core/data_container/message_queue.hpp"
    #include "emdevif/system/sys_message_slot.hpp"
    #include "emdevif/core/error_handler.hpp"
#endif

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
        emdevif::MessageSlot auto slot =
            emdevif::SysMessageSlot<int>(emdevif::SysQueueBuilder{.name = "BasicTestSlot"});
        static_assert(std::same_as<decltype(slot), emdevif::SysMessageSlot<int>>);

        emdevif_test_TestCaseContex contex = GET_THIS_TEST_CASE_CONTEXT();
        testImplWithoutCreate(contex, slot);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(FactoryConstruct)
    {
        emdevif::MessageSlot auto slot = emdevif::SysMessageSlot<int>{};
        static_assert(std::same_as<decltype(slot), emdevif::SysMessageSlot<int>>);

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
