/**
 * @file lock_guard.hpp
 * @brief 互斥锁封装器
 */

#pragma once
#ifndef EMDEVIF_CORE_RESOURCE_GUARD_LOCK_GUARD_HPP
    #define EMDEVIF_CORE_RESOURCE_GUARD_LOCK_GUARD_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/core/error_handler.hpp"

        #include <utility>
        #include <concepts>
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

/**
 * @brief 互斥锁守卫标记：构造时不自动加锁
 */
enum class LockGuardDoNotLockWhenInit {
    tag_value_
};

/// 互斥锁守卫标记常量（构造时不自动加锁）
constexpr auto lock_guard_do_not_lock_when_init = LockGuardDoNotLockWhenInit::tag_value_;

/**
 * @brief 有效互斥锁类型概念
 * @tparam LockType 互斥锁类型
 */
template<typename LockType>
concept ValidLock = requires(LockType lock_instance) {
    { lock_instance.lock() } -> std::same_as<ErrorCode>;
    { lock_instance.lock(1) } -> std::same_as<ErrorCode>;
    { lock_instance.try_lock() } -> std::same_as<ErrorCode>;
    { lock_instance.unlock() } -> std::same_as<void>;
};

/**
 * @brief RAII 风格的互斥锁守卫（构造时加锁，析构时解锁）
 * @tparam LockType 互斥锁类型
 */
template<ValidLock LockType>
class LockGuard final
{
public:
    LockGuard() = delete;
    LockGuard(const LockGuard&) = delete;
    LockGuard(LockGuard&&) = delete;

private:
    LockType& lock_instance_;  ///< 互斥锁实例的引用

public:
    /**
     * 构造 LockGuard 并自动加锁
     * @tparam Args 传递给 lock 的参数类型
     * @param lock_instance 互斥锁实例
     * @param args 传递给 lock 的参数
     * @throws ErrorWithCodeException 如果加锁失败
     */
    template<typename... Args>
    explicit LockGuard(LockType& lock_instance, Args&&... args) : lock_instance_(lock_instance)
    {
        auto ret = lock_instance_.lock(std::forward<Args>(args)...);
        if (ret != ErrorCode::Success) {
            throw ErrorWithCodeException(ret, "Failed to lock");
        }
    }
    /**
     * @overload
     * 构造 LockGuard 但不自动加锁
     * @param LockGuardDoNotLockWhenInit 不加锁标记
     * @param lock_instance 互斥锁实例
     */
    explicit LockGuard(LockGuardDoNotLockWhenInit, LockType& lock_instance) noexcept : lock_instance_(lock_instance) {}

    /**
     * 手动对被托管的互斥锁实例加锁
     * @tparam Args 传递给被托管对象的 lock 方法的参数类型
     * @param args 传递给被托管对象的 lock 方法的参数
     * @return 错误码
     */
    template<typename... Args>
    ErrorCode lock(Args&&... args) const noexcept
    {
        return lock_instance_.lock(std::forward<Args>(args)...);
    }

    /**
     * 手动对被托管的互斥锁实例尝试加锁
     * @tparam Args 传递给被托管对象的 try_lock 方法的参数类型
     * @param args 传递给被托管对象的 try_lock 方法的参数
     * @return 错误码
     */
    template<typename... Args>
    ErrorCode try_lock(Args&&... args) const noexcept
    {
        return lock_instance_.try_lock(std::forward<Args>(args)...);
    }

    ~LockGuard() noexcept
    {
        lock_instance_.unlock();
    }
};

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_RESOURCE_GUARD_LOCK_GUARD_HPP
