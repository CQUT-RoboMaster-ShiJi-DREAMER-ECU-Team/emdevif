/**
 * @file thread_test.cpp
 * @author DuYicheng
 * @date 2025-08-01
 * @brief 对线程接口的测试
 */

#include "emdevif_test_framework.h"
#include "cmsis_os2.h"

import emdevif.thread;
import emdevif.mutex;

template<typename T>
class MutexGuard
{
public:
    void set(T value)
    {
        mutex.lock();
        v = value;
        mutex.unlock();
    }

    T get()
    {
        volatile T ret{};

        mutex.lock();
        ret = v;
        mutex.unlock();

        return ret;
    }

    MutexGuard() : mutex(emdevif::Mutex::create({.name = "MutexGuard"})) {}

    ~MutexGuard()
    {
        mutex.destroy();
    }

private:
    emdevif::Mutex mutex;
    T v{};
};

TEST_SUIT(ThreadTest)
{
    TEST_CASE_BEGIN(Test1)
    {
        MutexGuard<int32_t> counter;

        auto a_thread = emdevif::Thread(
            [](void* param) {
                auto& cnt = *static_cast<MutexGuard<int32_t>*>(param);

                for (uint8_t i = 0; i < 100; i++) {
                    cnt.set(cnt.get() + 1);
                }

                emdevif::Thread::exit();
            },
            &counter,
            {.name = "Thread Test 1 a", .priority = osPriorityNormal, .stack_size = 128});

        auto b_thread = emdevif::Thread(
            [](void* param) {
                auto& cnt = *static_cast<MutexGuard<int32_t>*>(param);

                for (uint16_t i = 0; i < 353; i++) {
                    cnt.set(cnt.get() + 1);
                }

                emdevif::Thread::exit();
            },
            &counter,
            {.name = "Thread Test 1 b", .priority = osPriorityNormal, .stack_size = 128});

        emdevif::Thread::delayMs(1000);

        INT_ASSERT_EQ(counter.get(), 353 + 100, "");
    }
    TEST_CASE_END();
}

void threadTest()
{
    RUN_SUIT(ThreadTest);
}
