/**
 * @file mutex_interface.cppm
 * @author DuYicheng
 * @date 2025-08-01
 * @brief 互斥锁接口
 */

module;

#include <cstdint>
#include <optional>

#include "emdevif/fatal_handler.h"

export module emdevif.sys.mutex:interface;

import emdevif.errorHandler;
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
    ErrorCode lock(SysTick_t timeout_tick = Thread::maxDelay());

    [[nodiscard]] ErrorCode try_lock()
    {
        return lock(0U);
    }

    void unlock();

    [[nodiscard]] Handle getHandle() const
    {
        return handle_;
    }

    Mutex() : handle_(nullptr) {}

    explicit Mutex(const StronglyTypedHandle strongly_handle) : handle_(strongly_handle.value) {}

    Mutex& operator=(const Mutex&) = delete;
    Mutex(const Mutex&) = delete;

    Mutex& operator=(const StronglyTypedHandle strongly_handle)
    {
        if (handle_ != nullptr) {
            EMDEVIF_FATAL_HANDLER("Should not create mutex on non-deleted mutex!");
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
