/**
 * @file thread_test.cpp
 * @author DuYicheng
 * @date 2025-08-01
 * @brief 对线程接口的测试
 */

#include "emdevif_test_framework.h"
#include "cmsis_os2.h"

#include <utility>

import emdevif.thread;
import emdevif.mutex;

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

    T get()
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

    emdevif::Mutex mutex{};
    T v;
};

TEST_SUIT(ThreadTest)
{
    TEST_CASE_BEGIN(BasicTest)
    {
        MutexGuard<int32_t> counter{0};
        ASSERT_TRUE(counter.mutex.getHandle() != nullptr, "Mutex Create Failed!");

        struct Arg {
            emdevif::Thread* thd;
            decltype(counter)& cnt;
        };

        emdevif::Thread a_thread;
        Arg arga{&a_thread, counter};
        a_thread = emdevif::Thread::create(
            {.name = "Thread BasicTest a", .priority = osPriorityNormal, .stack_size = 128},
            [](void* param) {
                emdevif::Thread::delay(1);

                const auto arg = *static_cast<Arg*>(param);

                for (uint8_t i = 0; i < 100; i++) {
                    arg.cnt.set(arg.cnt.get() + 1);
                }

                arg.thd->exit();
            },
            &arga);
        ASSERT_TRUE(a_thread.getHandle() != nullptr, "Thread Create failed!");

        emdevif::Thread b_thread;
        Arg argb{&b_thread, counter};
        b_thread = emdevif::Thread::create(
            {.name = "Thread BasicTest b", .priority = osPriorityNormal, .stack_size = 128},
            [](void* param) {
                emdevif::Thread::delay(1);

                const auto arg = *static_cast<Arg*>(param);

                for (uint16_t i = 0; i < 353; i++) {
                    arg.cnt.set(arg.cnt.get() + 1);
                }

                arg.thd->exit();
            },
            &argb);
        ASSERT_TRUE(b_thread.getHandle() != nullptr, "Thread Create failed!");
        ASSERT_TRUE(a_thread.getHandle() != b_thread.getHandle(),
                    "a_thread = %p, b_thread = %p",
                    a_thread.getHandle(),
                    b_thread.getHandle());

        emdevif::Thread::delayMs(1000);

        INT_ASSERT_EQ(counter.get(), 353 + 100, "");
        ASSERT_TRUE(a_thread.getHandle() == nullptr && b_thread.getHandle() == nullptr,
                    "a_thread = %p, b_thread = %p",
                    a_thread.getHandle(),
                    b_thread.getHandle());
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(AssignAndMoveTest)
    {
        MutexGuard<int32_t> counter{0};

        ASSERT_TRUE(counter.mutex.getHandle() != nullptr, "Mutex Create Failed!");

        [[maybe_unused]] emdevif::Thread a_thread;
        [[maybe_unused]] emdevif::Thread b_thread;

        [&]() {
            std::pair<emdevif::Thread&, decltype(counter)> pa{a_thread, counter};
            a_thread = emdevif::Thread::create(
                {.name = "Thread Test 1 a", .priority = osPriorityNormal, .stack_size = 128},
                [](void* param) {
                    auto& [thread, cnt] = *static_cast<std::pair<emdevif::Thread&, decltype(counter)>*>(param);

                    for (uint8_t i = 0; i < 114; i++) {
                        cnt.set(cnt.get() + 1);
                    }

                    thread.exit();
                },
                &pa);

            std::pair<emdevif::Thread&, decltype(counter)> pb{b_thread, counter};
            b_thread = emdevif::Thread::create(
                {.name = "Thread Test 1 b", .priority = osPriorityNormal, .stack_size = 128},
                [](void* param) {
                    auto& [thread, cnt] = *static_cast<std::pair<emdevif::Thread&, decltype(counter)>*>(param);

                    for (uint16_t i = 0; i < 514; i++) {
                        cnt.set(cnt.get() + 1);
                    }

                    thread.exit();
                },
                &pb);
        }();
        emdevif::Thread::delayMs(1000);

        ASSERT_TRUE(a_thread.getHandle() != b_thread.getHandle(), "");

        emdevif::Thread a = std::move(a_thread);
        emdevif::Thread b = std::move(b_thread);
        ASSERT_TRUE(a_thread.getHandle() == nullptr, "");
        ASSERT_TRUE(b_thread.getHandle() == nullptr, "");

        INT_ASSERT_EQ(counter.get(), 114 + 514, "");
        ASSERT_TRUE(a.getHandle() != b.getHandle(), "");
    }
    TEST_CASE_END();
}

void threadTest()
{
    RUN_SUIT(ThreadTest);
}
