/**
 * @file resource_guard_test.cpp
 * @brief resource_guard 模块测试
 */

#include <cstdint>

#include <utility>
#ifdef __cpp_exceptions
    #include <stdexcept>
#endif

#include "emdevif_test_framework.h"

#ifdef EMDEVIF_USE_MODULES
import emdevif.core.resource_guard;
import emdevif.core.error_handler;
#else
    #include "emdevif/core/resource_guard.hpp"
    #include "emdevif/core/error_handler.hpp"
#endif

using namespace emdevif;

// ============================================================================
// Defer 测试辅助
// ============================================================================

namespace {

static int defer_counter = 0;

void increment_counter() noexcept
{
    ++defer_counter;
}

void reset_counter() noexcept
{
    defer_counter = 0;
}

}  // namespace

// ============================================================================
// Defer 测试
// ============================================================================

TEST_SUIT(DeferTest)
{
    TEST_CASE_BEGIN(BasicDeferredExecution)
    {
        reset_counter();

        {
            Defer d(&increment_counter);
            INT_EXPECT_EQ(defer_counter, 0);
        }

        INT_EXPECT_EQ(defer_counter, 1);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(DeferWithLambda)
    {
        int value = 0;

        {
            Defer d([&value]() noexcept { value = 42; });
            INT_EXPECT_EQ(value, 0);
        }

        INT_EXPECT_EQ(value, 42);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(MultipleDefersInReverseOrder)
    {
        int order[3] = {0, 0, 0};
        int step = 0;

        {
            Defer d1([&order, &step]() noexcept { order[step++] = 1; });
            Defer d2([&order, &step]() noexcept { order[step++] = 2; });
            Defer d3([&order, &step]() noexcept { order[step++] = 3; });

            INT_EXPECT_EQ(step, 0);
        }

        // 析构顺序为构造的逆序
        INT_EXPECT_EQ(order[0], 3);
        INT_EXPECT_EQ(order[1], 2);
        INT_EXPECT_EQ(order[2], 1);
        INT_EXPECT_EQ(step, 3);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(NestedDefer)
    {
        int value = 0;

        {
            Defer d1([&value]() noexcept { value += 10; });

            {
                Defer d2([&value]() noexcept { value += 1; });
                INT_EXPECT_EQ(value, 0);
            }

            // d2 已析构
            INT_EXPECT_EQ(value, 1);
        }

        // d1 已析构
        INT_EXPECT_EQ(value, 11);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(MoveConstructedFunctor)
    {
        int call_count = 0;
        auto func = [&call_count]() noexcept { ++call_count; };

        {
            Defer d(std::move(func));
            INT_EXPECT_EQ(call_count, 0);
        }

        INT_EXPECT_EQ(call_count, 1);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(ConstexprDefer)
    {
        // constexpr 构造：需要在常量表达式中验证
        constexpr auto test_constexpr = []() constexpr -> bool {
            int flag = 0;
            {
                Defer d([&flag]() noexcept { flag = 1; });
                if (flag != 0) return false;
            }
            return flag == 1;
        };

        EXPECT_TRUE(test_constexpr());
        static_assert(test_constexpr());
    }
    TEST_CASE_END();
}

// ============================================================================
// LockGuard 测试辅助
// ============================================================================

namespace {

class MockLock
{
public:
    int lock_count = 0;
    int try_lock_count = 0;
    int unlock_count = 0;
    ErrorCode lock_return = ErrorCode::Success;
    ErrorCode try_lock_return = ErrorCode::Success;

    ErrorCode lock() noexcept
    {
        ++lock_count;
        return lock_return;
    }

    ErrorCode lock(int) noexcept
    {
        ++lock_count;
        return lock_return;
    }

    ErrorCode try_lock() noexcept
    {
        ++try_lock_count;
        return try_lock_return;
    }

    void unlock() noexcept
    {
        ++unlock_count;
    }
};

}  // namespace

// ============================================================================
// LockGuard 测试
// ============================================================================

TEST_SUIT(LockGuardTest)
{
#ifdef __cpp_exceptions
    TEST_CASE_BEGIN(AutoLockOnConstruct)
    {
        MockLock lock;

        {
            LockGuard guard(lock);
            INT_EXPECT_EQ(lock.lock_count, 1);
            INT_EXPECT_EQ(lock.unlock_count, 0);
        }

        INT_EXPECT_EQ(lock.unlock_count, 1);
    }
    TEST_CASE_END();
#endif

    TEST_CASE_BEGIN(DoNotLockOnConstruct)
    {
        MockLock lock;

        {
            LockGuard guard(lock_guard_do_not_lock_when_init, lock);
            INT_EXPECT_EQ(lock.lock_count, 0);
            INT_EXPECT_EQ(lock.unlock_count, 0);
        }

        // 析构时仍会调用 unlock
        INT_EXPECT_EQ(lock.unlock_count, 1);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(ManualLock)
    {
        MockLock lock;

        {
            LockGuard guard(lock_guard_do_not_lock_when_init, lock);
            INT_EXPECT_EQ(lock.lock_count, 0);

            ErrorCode ret = guard.lock();
            INT_EXPECT_EQ(static_cast<int>(ret), static_cast<int>(ErrorCode::Success));
            INT_EXPECT_EQ(lock.lock_count, 1);
        }

        INT_EXPECT_EQ(lock.unlock_count, 1);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(ManualLockWithArgs)
    {
        MockLock lock;

        {
            LockGuard guard(lock_guard_do_not_lock_when_init, lock);
            ErrorCode ret = guard.lock(42);
            INT_EXPECT_EQ(static_cast<int>(ret), static_cast<int>(ErrorCode::Success));
            INT_EXPECT_EQ(lock.lock_count, 1);
        }

        INT_EXPECT_EQ(lock.unlock_count, 1);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(TryLockSuccess)
    {
        MockLock lock;

        {
            LockGuard guard(lock_guard_do_not_lock_when_init, lock);
            ErrorCode ret = guard.try_lock();
            INT_EXPECT_EQ(static_cast<int>(ret), static_cast<int>(ErrorCode::Success));
            INT_EXPECT_EQ(lock.try_lock_count, 1);
            INT_EXPECT_EQ(lock.lock_count, 0);
        }

        INT_EXPECT_EQ(lock.unlock_count, 1);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(TryLockFailure)
    {
        MockLock lock;
        lock.try_lock_return = ErrorCode::Timeout;

        {
            LockGuard guard(lock_guard_do_not_lock_when_init, lock);
            ErrorCode ret = guard.try_lock();
            INT_EXPECT_EQ(static_cast<int>(ret), static_cast<int>(ErrorCode::Timeout));
            INT_EXPECT_EQ(lock.try_lock_count, 1);
        }

        INT_EXPECT_EQ(lock.unlock_count, 1);
    }
    TEST_CASE_END();

#ifdef __cpp_exceptions
    TEST_CASE_BEGIN(AutoLockFailureThrows)
    {
        MockLock lock;
        lock.lock_return = ErrorCode::Timeout;

        bool exception_caught = false;
        try {
            LockGuard guard(lock);
        }
        catch (const ErrorWithCodeException& e) {
            exception_caught = true;
            INT_EXPECT_EQ(static_cast<int>(e.getErrorCode()), static_cast<int>(ErrorCode::Timeout));
        }

        EXPECT_TRUE(exception_caught);
    }
    TEST_CASE_END();
#endif
}

// ============================================================================
// InitGuard 测试辅助
// ============================================================================

namespace {

class MockDevice
{
public:
    bool initialized = false;
    bool deinitialized = false;
    ErrorCode init_return = ErrorCode::Success;
    int init_arg = 0;

    ErrorCode init() noexcept
    {
        initialized = true;
        return init_return;
    }

    ErrorCode init(int arg) noexcept
    {
        initialized = true;
        init_arg = arg;
        return init_return;
    }

    void deInit() noexcept
    {
        deinitialized = true;
    }
};

}  // namespace

// ============================================================================
// InitGuard 测试
// ============================================================================

TEST_SUIT(InitGuardTest)
{
#ifdef __cpp_exceptions
    TEST_CASE_BEGIN(AutoInitOnConstruct)
    {
        MockDevice dev;

        {
            InitGuard guard(dev);
            EXPECT_TRUE(dev.initialized);
            EXPECT_FALSE(dev.deinitialized);
        }

        EXPECT_TRUE(dev.deinitialized);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(AutoInitOnConstructWithArgs)
    {
        MockDevice dev;

        {
            InitGuard guard(dev, 42);
            EXPECT_TRUE(dev.initialized);
            INT_EXPECT_EQ(dev.init_arg, 42);
        }

        EXPECT_TRUE(dev.deinitialized);
    }
    TEST_CASE_END();
#endif

    TEST_CASE_BEGIN(DoNotInitOnConstruct)
    {
        MockDevice dev;

        {
            InitGuard guard(init_guard_do_not_init_object_tag, dev);
            EXPECT_FALSE(dev.initialized);
            EXPECT_FALSE(dev.deinitialized);
        }

        EXPECT_TRUE(dev.deinitialized);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(ManualInit)
    {
        MockDevice dev;

        {
            InitGuard guard(init_guard_do_not_init_object_tag, dev);
            EXPECT_FALSE(dev.initialized);

            ErrorCode ret = guard.init();
            INT_EXPECT_EQ(static_cast<int>(ret), static_cast<int>(ErrorCode::Success));
            EXPECT_TRUE(dev.initialized);
        }

        EXPECT_TRUE(dev.deinitialized);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(ManualInitWithArgs)
    {
        MockDevice dev;

        {
            InitGuard guard(init_guard_do_not_init_object_tag, dev);
            ErrorCode ret = guard.init(99);
            INT_EXPECT_EQ(static_cast<int>(ret), static_cast<int>(ErrorCode::Success));
            EXPECT_TRUE(dev.initialized);
            INT_EXPECT_EQ(dev.init_arg, 99);
        }

        EXPECT_TRUE(dev.deinitialized);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(ManualInitFailure)
    {
        MockDevice dev;
        dev.init_return = ErrorCode::OperationFail;

        {
            InitGuard guard(init_guard_do_not_init_object_tag, dev);
            ErrorCode ret = guard.init();
            INT_EXPECT_EQ(static_cast<int>(ret), static_cast<int>(ErrorCode::OperationFail));
            EXPECT_FALSE(dev.initialized);
        }

        EXPECT_TRUE(dev.deinitialized);
    }
    TEST_CASE_END();

#ifdef __cpp_exceptions
    TEST_CASE_BEGIN(AutoInitFailureThrows)
    {
        MockDevice dev;
        dev.init_return = ErrorCode::OutOfMemory;

        bool exception_caught = false;
        try {
            InitGuard guard(dev);
        }
        catch (const ErrorWithCodeException& e) {
            exception_caught = true;
            INT_EXPECT_EQ(static_cast<int>(e.getErrorCode()), static_cast<int>(ErrorCode::OutOfMemory));
        }

        EXPECT_TRUE(exception_caught);
        // auto-init 失败，对象未初始化，但 deInit 仍会在析构时被调用
        EXPECT_TRUE(dev.deinitialized);
    }
    TEST_CASE_END();
#endif
}

// ============================================================================
// tryFinally 测试
// ============================================================================

TEST_SUIT(TryFinallyTest)
{
    TEST_CASE_BEGIN(BasicTryFinally)
    {
        int finally_ran = 0;

        int result = tryFinally([&]() { return 42; }, [&]() noexcept { finally_ran = 1; });

        INT_EXPECT_EQ(result, 42);
        INT_EXPECT_EQ(finally_ran, 1);
    }
    TEST_CASE_END();

#ifdef __cpp_exceptions
    TEST_CASE_BEGIN(FinallyExecutesOnException)
    {
        int finally_ran = 0;
        bool exception_caught = false;

        try {
            tryFinally([&]() -> int { throw std::runtime_error("test error"); }, [&]() noexcept { finally_ran = 1; });
        }
        catch (const std::runtime_error&) {
            exception_caught = true;
        }

        EXPECT_TRUE(exception_caught);
        INT_EXPECT_EQ(finally_ran, 1);
    }
    TEST_CASE_END();
#endif

    TEST_CASE_BEGIN(FinallyExecutesOnReturn)
    {
        int finally_ran = 0;
        int side_effect = 0;

        int result = tryFinally(
            [&]() {
                side_effect = 100;
                return side_effect;
            },
            [&]() noexcept { finally_ran = 1; });

        INT_EXPECT_EQ(result, 100);
        INT_EXPECT_EQ(side_effect, 100);
        INT_EXPECT_EQ(finally_ran, 1);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(VoidReturnType)
    {
        int finally_ran = 0;
        int side_effect = 0;

        tryFinally([&]() { side_effect = 77; }, [&]() noexcept { finally_ran = 1; });

        INT_EXPECT_EQ(side_effect, 77);
        INT_EXPECT_EQ(finally_ran, 1);
    }
    TEST_CASE_END();
}

// ============================================================================
// 测试入口
// ============================================================================

void resourceGuardTest()
{
    RUN_SUIT(DeferTest);
    RUN_SUIT(LockGuardTest);
    RUN_SUIT(InitGuardTest);
    RUN_SUIT(TryFinallyTest);
}
