/**
 * @file mutex_interface.cppm
 * @module emdevif.sys.mutex:interface
 * @author DuYicheng
 * @date 2025-08-01
 * @brief 互斥锁接口
 */

module;

#include <cstdint>
#include <optional>

#include "emdevif/fault_handler.hpp"

export module emdevif.sys.mutex:interface;

import emdevif.error_handler;
import emdevif.sys.thread;

export namespace emdevif {

class Mutex
{
public:
    using Handle = void*;

    class StaticInstance;

    struct Attribute {
        const char* name{};                        ///< 名称

        StaticInstance* static_instance{nullptr};  ///< 静态实例内存
        std::size_t instance_size{0U};             ///< 实例大小
    };

private:
    struct StronglyTypedHandle {
        Handle value;
    };

public:
    static StronglyTypedHandle create(const Attribute& attribute);

    static void destroy(Mutex& obj);

    void destroy()
    {
        destroy(*this);
        handle_ = nullptr;
    }

    // NOLINTNEXTLINE
    ErrorCode lock(SysTick_t timeout_tick = Thread::MAX_DELAY());

    [[nodiscard]] ErrorCode try_lock()
    {
        return lock(0U);
    }

    void unlock();

    [[nodiscard]] std::optional<Handle> getHandle() const
    {
        if (handle_ == nullptr) {
            return std::nullopt;
        }

        return handle_;
    }

    Mutex() : handle_(nullptr) {}

    explicit Mutex(const StronglyTypedHandle strongly_handle) : handle_(strongly_handle.value) {}

    Mutex& operator=(const Mutex&) = delete;
    Mutex(const Mutex&) = delete;

    Mutex& operator=(const StronglyTypedHandle strongly_handle)
    {
        if (handle_ != nullptr) {
            EMDEVIF_FAULT_HANDLER("Should not create mutex on non-deleted mutex!");
            return *this;
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    explicit Mutex(const Attribute& attribute) : Mutex(create(attribute)) {}

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

    ~Mutex();

private:
    Handle handle_;
};

}  // namespace emdevif
