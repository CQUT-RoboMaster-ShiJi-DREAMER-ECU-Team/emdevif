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

enum class LockGuardDoNotLockWhenInit {
    tag_value_
};

constexpr auto lock_guard_do_not_lock_when_init = LockGuardDoNotLockWhenInit::tag_value_;

template<typename LockType>
concept ValidLock = requires(LockType lock_instance) {
    { lock_instance.lock() } -> std::same_as<ErrorCode>;
    { lock_instance.lock(1) } -> std::same_as<ErrorCode>;
    { lock_instance.try_lock() } -> std::same_as<ErrorCode>;
    { lock_instance.unlock() } -> std::same_as<void>;
};

template<ValidLock LockType>
class LockGuard final
{
public:
    LockGuard() = delete;
    LockGuard(const LockGuard&) = delete;
    LockGuard(LockGuard&&) = delete;

private:
    LockType& lock_instance_;

public:
    template<typename... Args>
    explicit LockGuard(LockType& lock_instance, Args&&... args) : lock_instance_(lock_instance)
    {
        auto ret = lock_instance_.lock(std::forward<Args>(args)...);
        if (ret != ErrorCode::Success) {
            throw ErrorWithCodeException(ret, "Failed to lock");
        }
    }

    explicit LockGuard(LockGuardDoNotLockWhenInit, LockType& lock_instance) noexcept : lock_instance_(lock_instance) {}

    template<typename... Args>
    ErrorCode lock(Args&&... args) const noexcept
    {
        return lock_instance_.lock(std::forward<Args>(args)...);
    }

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
