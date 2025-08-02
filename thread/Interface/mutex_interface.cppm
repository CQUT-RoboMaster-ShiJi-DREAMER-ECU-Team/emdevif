/**
 * @file mutex_interface.cppm
 * @module emdevif.mutex:interface
 * @author DuYicheng
 * @date 2025-08-01
 * @brief 互斥锁接口
 */

module;

#include "emdevif/fault_handler.hpp"

#include <cstdint>

export module emdevif.mutex:interface;

import emdevif.error_handler;
import emdevif.thread;

export namespace emdevif {

class Mutex
{
public:
    using Handle = void*;

    struct Attribute {
        const char* name{};     ///< 名称
        void* cb_mem{nullptr};  ///< 控制块内存
        uint32_t cb_size{};     ///< 控制块大小
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
    ErrorCode lock(uint32_t timeout_tick = Thread::MAX_DELAY()) const;

    [[nodiscard]] ErrorCode try_lock() const;

    void unlock() const;

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
