/**
 * @file thread_mutex_test.cpp
 * @author DuYicheng
 * @date 2025-08-01
 * @brief 对线程接口的测试
 */

#include "emdevif_test_framework.h"

#include <cstdint>
#include <cmath>

#include <utility>
#include <functional>
#include <string>

#include "printf.h"

import emdevif.sys.thread;
import emdevif.sys.mutex;

template<typename T>
class MutexGuard
{
public:
    void set(T value)
    {
        mutex.lock();  // NOLINT
        v = value;
        mutex.unlock();
    }

    [[nodiscard]] T get() const
    {
        volatile T ret{};

        mutex.lock();  // NOLINT
        ret = v;
        mutex.unlock();

        return ret;
    }

    MutexGuard() : v()  // NOLINT(*-use-equals-default)
    {
        mutex = emdevif::Mutex::create({.name = "MutexGuard"});
    }

    explicit MutexGuard(const T value) : v(value)
    {
        mutex = emdevif::Mutex::create({.name = "MutexGuard"});
    }

    ~MutexGuard()  // NOLINT(*-use-equals-default)
    {
        mutex.destroy();
    }

    mutable emdevif::Mutex mutex{};
    volatile T v;
};

TEST_SUIT(MutexTest)
{
    TEST_CASE_BEGIN(BasicTest)
    {
        using emdevif::Mutex;

        Mutex mutex;
        ASSERT_TRUE(!mutex.getHandle().has_value(), "");

        mutex = Mutex::create({.name = "basic 1"});
        ASSERT_TRUE(mutex.getHandle().has_value(), "");

        auto ret = mutex.lock();
        ASSERT_TRUE(ret == emdevif::ErrorCode::Success, "Failed to lock!");
        mutex.unlock();

        ret = mutex.lock(0);
        ASSERT_TRUE(ret == emdevif::ErrorCode::Success, "Failed to lock!");
        mutex.unlock();
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(DirectlyTest)
    {
        using emdevif::Mutex;

        Mutex mutex({.name = "basic 1"});
        ASSERT_TRUE(mutex.getHandle().has_value(), "");

        auto ret = mutex.lock();
        ASSERT_TRUE(ret == emdevif::ErrorCode::Success, "Failed to lock!");
        mutex.unlock();

        ret = mutex.lock(0);
        ASSERT_TRUE(ret == emdevif::ErrorCode::Success, "Failed to lock!");
        mutex.unlock();

        mutex.destroy();
        ASSERT_TRUE(!mutex.getHandle().has_value(), "");
    }
    TEST_CASE_END();
}

TEST_SUIT(ThreadBasicTest)
{
    TEST_CASE_BEGIN(BasicTest)
    {
        MutexGuard<int32_t> counter{0};
        MutexGuard<uint8_t> flag{0};
        ASSERT_TRUE(counter.mutex.getHandle().has_value(), "Mutex Create Failed!");
        ASSERT_TRUE(flag.mutex.getHandle().has_value(), "Mutex Create Failed!");
        UINT_ASSERT_EQ(0, flag.get(), "");
        INT_ASSERT_EQ(0, counter.get(), "");

        struct Arg {
            emdevif::Thread* thd;
            decltype(counter)* cnt;
            decltype(flag)* flg;
        };

        emdevif::Thread a_thread;
        Arg arga{&a_thread, &counter, &flag};
        a_thread = emdevif::Thread::create(
            {.name = "Thread BasicTest a", .priority = emdevif::Thread::Priority::BelowNormal, .stack_size = 256},
            [](void* param) {
                auto [thd, cnt, flg] = *static_cast<Arg*>(param);

                while ((flg->get() & 0b0011) != 0b0011) {
                    emdevif::Thread::delay(5);
                }

                for (auto i = 0U; i < 100; i++) {
                    cnt->set(cnt->get() + 1);
                }

                flg->set(flg->get() | 0b0100);
                thd->exit();
            },
            &arga);
        ASSERT_TRUE(a_thread.getHandle().has_value(), "Thread Create failed!");

        emdevif::Thread b_thread;
        Arg argb{&b_thread, &counter, &flag};
        b_thread = emdevif::Thread::create(
            {.name = "Thread BasicTest b", .stack_size = 256},
            [](void* param) {
                auto [thd, cnt, flg] = *static_cast<Arg*>(param);

                while ((flg->get() & 0b0011) != 0b0011) {
                    emdevif::Thread::delay(5);
                }

                for (auto i = 0U; i < 353; i++) {
                    cnt->set(cnt->get() + 1);
                }

                flg->set(flg->get() | 0b1000);
                thd->exit();
            },
            &argb);
        ASSERT_TRUE(b_thread.getHandle().has_value(), "Thread Create failed!");
        ASSERT_TRUE(a_thread.getHandle().value_or(nullptr) != b_thread.getHandle().value_or(nullptr),
                    "a_thread = %p, b_thread = %p",
                    a_thread.getHandle().value_or(nullptr),
                    b_thread.getHandle().value_or(nullptr));

        flag.set(flag.get() | 0b0001);
        flag.set(flag.get() | 0b0010);

        // 考虑到不是所有 RTOS 都有 join 方法，并且这里仅做测试，
        // 不便于使用事件组。因此使用互斥锁+标志变量的方式实现条件阻塞。
        while (flag.get() != 0b1111) {
            emdevif::Thread::delay(1);
        }

        INT_EXPECT_EQ(counter.get(), 353 + 100);
        ASSERT_TRUE(
            a_thread.getHandle().value_or(nullptr) == nullptr && b_thread.getHandle().value_or(nullptr) == nullptr,
            "a_thread = %p, b_thread = %p",
            a_thread.getHandle().value_or(nullptr),
            b_thread.getHandle().value_or(nullptr));
    }
    TEST_CASE_END();
}

// 此处注意：这个测试样例必须单独拿出来作为一个测试套件（函数），不能和上一个放在一起（原因未知）
TEST_SUIT(ThreadAssignAndMoveTest)
{
    TEST_CASE_BEGIN(AssignAndMoveTest)
    {
        MutexGuard<int32_t> counter{0};
        MutexGuard<uint8_t> flag{0};
        ASSERT_TRUE(counter.mutex.getHandle().has_value(), "Mutex Create Failed!");
        ASSERT_TRUE(flag.mutex.getHandle().has_value(), "Mutex Create Failed!");

        struct Arg {
            emdevif::Thread& thd;
            decltype(counter)& cnt;
            decltype(flag)& flg;
        };

        emdevif::Thread a_thread;
        emdevif::Thread b_thread;

        emdevif::Thread a_;
        emdevif::Thread b_;

        ASSERT_TRUE(!a_.getHandle().has_value() && !b_.getHandle().has_value(), "");

        Arg arga{a_thread, counter, flag};
        a_thread = emdevif::Thread::create(
            {.name = "Thd Assign a", .priority = emdevif::Thread::Priority::BelowNormal, .stack_size = 256},
            [](void* param) {
                const auto [thd, cnt, flg] = *static_cast<Arg*>(param);

                while ((flg.get() & 0b0011) != 0b0011) {
                    emdevif::Thread::delay(1);
                }

                for (uint8_t i = 0; i < 100; i++) {
                    cnt.set(cnt.get() + 1);
                }

                flg.set(flg.get() | 0b0100);
                thd.exit();
            },
            &arga);
        ASSERT_TRUE(a_thread.getHandle().has_value(), "Thread Create failed!");

        Arg argb{b_thread, counter, flag};
        b_thread = emdevif::Thread::create(
            {.name = "Thd Assign b", .stack_size = 256},
            [](void* param) {
                const auto [thd, cnt, flg] = *static_cast<Arg*>(param);

                while ((flg.get() & 0b0011) != 0b0011) {
                    emdevif::Thread::delay(1);
                }

                for (uint16_t i = 0; i < 353; i++) {
                    cnt.set(cnt.get() + 1);
                }

                flg.set(flg.get() | 0b1000);
                thd.exit();
            },
            &argb);
        ASSERT_TRUE(b_thread.getHandle().has_value(), "Thread Create failed!");
        ASSERT_TRUE(a_thread.getHandle().value_or(nullptr) != b_thread.getHandle().value_or(nullptr),
                    "a_thread = %p, b_thread = %p",
                    a_thread.getHandle().value_or(nullptr),
                    b_thread.getHandle().value_or(nullptr));

        const auto [a_previous, b_previous] = std::pair{a_thread.getHandle(), b_thread.getHandle()};
        ASSERT_TRUE(a_previous.value_or(nullptr) == a_thread.getHandle().value_or(nullptr) &&
                        b_previous.value_or(nullptr) == b_thread.getHandle().value_or(nullptr),
                    "");
        ASSERT_TRUE(a_thread.getHandle().has_value() && b_thread.getHandle().has_value(), "");
        a_ = std::move(a_thread);
        b_ = std::move(b_thread);
        ASSERT_TRUE(!a_thread.getHandle().has_value() && !b_thread.getHandle().has_value(), "");
        ASSERT_TRUE(a_.getHandle().has_value() && b_.getHandle().has_value(), "");
        ASSERT_TRUE(a_.getHandle().value_or(nullptr) == a_previous.value_or(nullptr) &&
                        b_.getHandle().value_or(nullptr) == b_previous.value_or(nullptr),
                    "");

        flag.set(flag.get() | 0b0001);
        flag.set(flag.get() | 0b0010);
        while (flag.get() != 0b1111) {
            emdevif::Thread::delay(1);
        }

        INT_EXPECT_EQ(counter.get(), 353 + 100);
        ASSERT_TRUE(!a_thread.getHandle().has_value() && !b_thread.getHandle().has_value(),
                    "a_thread = %p, b_thread = %p",
                    a_thread.getHandle(),
                    b_thread.getHandle());
    }
    TEST_CASE_END();
}

TEST_SUIT(MulParamFuncTest)
{
    TEST_CASE_BEGIN(Test)
    {
        static bool is_pass = true;

        int outside = 42795;
        emdevif::Thread th;

        char error_msg[100];
        error_msg[0] = '\0';

        MutexGuard<uint8_t> flag{0b000};

        auto func = [outside, &th, &flag, &error_msg](const int a, const float f, int& ref) noexcept -> void {
            while (!(flag.get() & 0b011)) {
                emdevif::Thread::delay(1);
            }

            if (outside != 42795 || a != 123 || std::abs(f - 114.514f) > 0.0001f || ref != 5) {
                ::sprintf(error_msg, "outside = %d, a = %d, f = %f, ref = %d", outside, a, f, ref);
                is_pass = false;
            }

            ref = -352;

            flag.set(flag.get() | 0b100);

            th.exit();
        };

        int num_for_ref = 5;
        th = emdevif::Thread::create({.name = "MulParamThread", .stack_size = 128},
                                     emdevif::Thread::mulparam,
                                     func,
                                     123,
                                     114.514f,
                                     std::ref(num_for_ref));

        flag.set(flag.get() | 0b011);

        while (flag.get() != 0b111) {
            emdevif::Thread::delay(1);
        }

        ASSERT_TRUE(!th.getHandle().has_value(), "");
        INT_EXPECT_EQ(num_for_ref, -352);
        EXPECT_TRUE(is_pass)->MESSAGE("%s", error_msg);
    }
    TEST_CASE_END();
}

void threadAndMutexTest()
{
    RUN_SUIT(MutexTest);
    RUN_SUIT(ThreadBasicTest);
    RUN_SUIT(ThreadAssignAndMoveTest);
    RUN_SUIT(MulParamFuncTest);
}
