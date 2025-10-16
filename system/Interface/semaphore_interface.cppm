/**
 * @file semaphore_interface.cppm
 * @module emdevif.sys.semaphore:interface
 * @author DuYicheng
 * @date 2025-08-16
 * @brief 信号量接口
 */

module;

#include <cstddef>
#include <optional>

#include "emdevif/fatal_handler.h"

export module emdevif.sys.semaphore:interface;

export import emdevif.errorHandler;
export import emdevif.sys.thread;

export namespace emdevif {

template<std::ptrdiff_t least_max_value>
class CountingSemaphore
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
    static auto create(const Attribute& attribute) -> StronglyTypedHandle;

    static void destroy(CountingSemaphore& obj);

    void destroy()
    {
        destroy(*this);
        handle_ = nullptr;
    }

    static constexpr std::ptrdiff_t max = 1;

    void release(bool in_isr) noexcept;

    ErrorCode acquire(bool in_isr, SysTick_t timeout_tick = Thread::maxDelay()) noexcept;

    ErrorCode try_acquire(const bool in_isr) noexcept
    {
        return acquire(in_isr, 0U);
    }

    [[nodiscard]] std::optional<Handle> getHandle() const
    {
        if (handle_ == nullptr) {
            return std::nullopt;
        }

        return handle_;
    }

    CountingSemaphore() : handle_(nullptr) {}

    explicit CountingSemaphore(const StronglyTypedHandle strongly_handle) : handle_(strongly_handle.value) {}

    CountingSemaphore& operator=(const CountingSemaphore&) = delete;
    CountingSemaphore(const CountingSemaphore&) = delete;

    CountingSemaphore& operator=(const StronglyTypedHandle strongly_handle)
    {
        if (handle_ != nullptr) {
            EMDEVIF_FATAL_HANDLER("Should not create counting semaphore on non-deleted counting semaphore!");
            return *this;
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    explicit CountingSemaphore(const Attribute& attribute) : CountingSemaphore(create(attribute)) {}

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

    ~CountingSemaphore();

private:
    Handle handle_;
};

using BinarySemaphore = CountingSemaphore<1>;

}  // namespace emdevif
