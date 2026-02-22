/**
 * @file mutex_interface.cppm
 * @brief 互斥锁接口
 */

// ReSharper disable CppDoxygenUnresolvedReference

module;

#include <cstdint>

#include "emdevif/core/fatal_handler.h"

export module emdevif.sys.mutex:interface;

import emdevif.core.error_handler;
import emdevif.sys.thread;

export namespace emdevif {

/**
 * 互斥锁
 */
class Mutex
{
public:
    using Handle = void*;  ///< 底层实现的句柄

    class StaticInstance;

    /// 属性
    struct Attribute {
        const char* name{};                        ///< 名称

        StaticInstance* static_instance{nullptr};  ///< 静态实例内存
    };

private:
    /**
     * 强类型句柄
     * @copydoc sys_strongly_typed_handle
     */
    struct StronglyTypedHandle {
        Handle value;
    };

public:
    // todo 这里的注释中的示例仿照 thread 里的来写
    /**
     * 创建互斥锁
     * @param attribute 属性
     * @return 创建好的强类型句柄
     */
    static StronglyTypedHandle create(const Attribute& attribute) noexcept;

    /**
     * 销毁互斥锁
     * @param obj 待销毁的互斥锁
     */
    static void destroy(Mutex& obj) noexcept;

    /**
     * 销毁互斥锁（自身）
     */
    void destroy() noexcept
    {
        destroy(*this);
        handle_ = nullptr;
    }

    /**
     * 获取互斥锁，如果已经被占用，则等待
     * @param timeout_tick 等待时间，默认无限等待
     * @retval ErrorCode::Success 成功锁定
     * @retval ErrorCode::Timeout 等待超时
     */
    ErrorCode lock(SysTick_t timeout_tick = Thread::maxDelay()) noexcept;

    /**
     * 尝试获取互斥锁（不会等待。如果失败，立刻返回）
     * @retval ErrorCode::Success 成功锁定
     * @retval ErrorCode::Timeout 等待超时
     */
    [[nodiscard]] ErrorCode try_lock() noexcept
    {
        return lock(0U);
    }

    /**
     * 释放互斥锁
     */
    void unlock() noexcept;

    /**
     * 获取底层实现的句柄
     * @return 底层实现的句柄
     */
    [[nodiscard]] Handle getHandle() const noexcept
    {
        return handle_;
    }

    Mutex() noexcept : handle_(nullptr) {}

    explicit Mutex(const StronglyTypedHandle strongly_handle) noexcept : handle_(strongly_handle.value) {}

    Mutex& operator=(const Mutex&) = delete;
    Mutex(const Mutex&) = delete;

    Mutex& operator=(const StronglyTypedHandle strongly_handle) noexcept
    {
        if (handle_ != nullptr) {
            EMDEVIF_FATAL_HANDLER("Should not create mutex on non-deleted mutex!");
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    explicit Mutex(const Attribute& attribute) noexcept : Mutex(create(attribute)) {}

    Mutex(Mutex&& other) noexcept : handle_(other.handle_)
    {
        other.handle_ = nullptr;
    }

    Mutex& operator=(Mutex&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        this->handle_ = other.handle_;
        other.handle_ = nullptr;

        return *this;
    }

    ~Mutex() noexcept;

private:
    Handle handle_;  ///< 底层实现的句柄
};

}  // namespace emdevif
