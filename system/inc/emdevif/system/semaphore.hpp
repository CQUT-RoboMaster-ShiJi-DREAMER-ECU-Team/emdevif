/**
 * @file semaphore.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_SYSTEM_SEMAPHORE_HPP
#define EMDEVIF_SYSTEM_SEMAPHORE_HPP

#include "emdevif/core/detail/config.hpp"

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#include "emdevif/core/fatal_handler.h"

#include "emdevif/core/error_handler.hpp"
#include "emdevif/system/thread.hpp"

#include <cstddef>
#endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

/**
 * 计数信号量
 * @tparam least_max_value 信号量计数最大值
 */
template<std::ptrdiff_t least_max_value = 1>
class CountingSemaphore
{
public:
    using Handle = void*;  ///< 底层实现句柄

    /**
     * 静态实例
     * @copydoc sys_static_instance
     */
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
    /**
     * 创建信号量
     *
     * 静态创建使用示例：
     * @code{.cpp}
     * import emdevif.sys.semaphore;
     *
     * void func(void*)
     * {
     *     auto semaphore = emdevif::CountingSemaphore<2>::create({.name = "semaphore"});
     *     // 动态创建计数最大值为 2 的信号量
     *
     *     // ...
     *
     *     // 离开作用域后自动销毁
     * }
     * @endcode
     *
     * 动态创建使用示例：
     * @code{.cpp}
     * import emdevif.sys.semaphore;
     *
     * emdevif::CountingSemaphore<2> semaphore;
     * emdevif::CountingSemaphore<2>::StaticInstance semaphore_instance;
     *
     * void func(void*)
     * {
     *     semaphore = emdevif::CountingSemaphore<2>::create({
     *         .name = "semaphore",
     *         .static_instance = &semaphore_instance});
     *     // 静态创建计数最大值为 2 的信号量
     *
     *     // 静态创建的实例不能删除
     *     while (true) {
     *         // ...
     *     }
     * }
     * @endcode
     * @param attribute 属性
     * @return 创建的强类型句柄
     */
    static auto create(const Attribute& attribute) noexcept -> StronglyTypedHandle;

    /**
     * 销毁信号量
     * @param obj 待销毁的信号量
     */
    static void destroy(CountingSemaphore& obj) noexcept;

    /**
     * 销毁信号量（自身）
     */
    void destroy() noexcept
    {
        destroy(*this);
        handle_ = nullptr;
    }

    /**
     * 释放信号量
     * @param in_isr 是否处于中断上下文
     */
    void release(bool in_isr) noexcept;

    /**
     * 获取信号量
     * @param in_isr 是否处于中断上下文
     * @param timeout_tick 超时时间，默认无限等待。如果是在中断内，该参数无效。
     * @retval ErrorCode::Success 成功获取信号量
     * @retval ErrorCode::Timeout 未处于中断上下文时，超时了
     * @retval ErrorCode::OperationFail 处于中断上下文时，获取信号量失败
     */
    ErrorCode acquire(bool in_isr, SysTick_t timeout_tick = Thread::maxDelay()) noexcept;

    /**
     * 尝试获取信号量（不会等待，如果失败立刻返回）
     * @param in_isr 是否处于中断上下文
     * @retval ErrorCode::Success 成功获取信号量
     * @retval ErrorCode::Timeout 未处于中断上下文时，获取失败
     * @retval ErrorCode::OperationFail 处于中断上下文时，获取失败
     */
    ErrorCode try_acquire(const bool in_isr) noexcept
    {
        return acquire(in_isr, 0U);
    }

    /**
     * 获取底层实现所用的句柄
     * @return 底层句柄
     */
    [[nodiscard]] Handle getHandle() const noexcept
    {
        return handle_;
    }

    CountingSemaphore() noexcept : handle_(nullptr) {}

    explicit CountingSemaphore(const StronglyTypedHandle strongly_handle) noexcept : handle_(strongly_handle.value) {}

    CountingSemaphore& operator=(const CountingSemaphore&) = delete;
    CountingSemaphore(const CountingSemaphore&) = delete;

    CountingSemaphore& operator=(const StronglyTypedHandle strongly_handle) noexcept
    {
        if (handle_ != nullptr) {
            EMDEVIF_FATAL_HANDLER("Should not create counting semaphore on non-deleted counting semaphore!");
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    explicit CountingSemaphore(const Attribute& attribute) noexcept : CountingSemaphore(create(attribute)) {}

    CountingSemaphore(CountingSemaphore&& other) noexcept : handle_(other.handle_)
    {
        other.handle_ = nullptr;
    }

    CountingSemaphore& operator=(CountingSemaphore&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        this->handle_ = other.handle_;
        other.handle_ = nullptr;

        return *this;
    }

    ~CountingSemaphore() noexcept;

private:
    Handle handle_;  ///< 底层实现所用的句柄
};

/// 二值信号量
using BinarySemaphore = CountingSemaphore<1>;

}  // namespace emdevif

#include "emdevif/system_impl/semaphore.inl"

#endif  // !EMDEVIF_SYSTEM_SEMAPHORE_HPP
