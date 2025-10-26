/**
 * @file event_group_interface.cppm
 * @author DuYicheng
 * @date 2025-08-16
 * @brief 事件组接口
 */

module;

#include <cstdint>

#include "emdevif/fatal_handler.h"

export module emdevif.sys.eventGroup:interface;
import :definitions;

export import emdevif.errorHandler;
export import emdevif.sys.thread;
export import emdevif.util.bitInt;
import emdevif.sys.thread;

export namespace emdevif {

class EventGroup
{
public:
    using Handle = void*;

    class StaticInstance;

    struct Attribute {
        const char* name{};                        ///< 名称

        StaticInstance* static_instance{nullptr};  ///< 静态实例内存
    };

private:
    struct StronglyTypedHandle {
        Handle value;
    };

public:
    static StronglyTypedHandle create(const Attribute& attribute);

    static void destroy(EventGroup& obj);

    void destroy()
    {
        destroy(*this);
        handle_ = nullptr;
    }

    static constexpr BitsType_t maxEventBits = internal::maxEventBits();

    using EventBits_t = UBitInt<maxEventBits>;

    auto getBits(bool in_isr) noexcept -> EventBits_t;

    auto setBits(bool in_isr, EventBits_t bits) noexcept -> EventBits_t;

    auto clearBits(bool in_isr, EventBits_t bits) noexcept -> EventBits_t;

    auto waitBits(EventBits_t bits_wait_for,
                  bool clear_on_exit,
                  bool wait_for_all_bits,
                  SysTick_t timeout_tick = Thread::maxDelay()) noexcept -> EventBits_t;

    EventBits_t sync(EventBits_t bits_to_set,
                     EventBits_t bits_wait_for,
                     SysTick_t timeout_tick = Thread::maxDelay()) noexcept;

    [[nodiscard]] Handle getHandle() const
    {
        return handle_;
    }

    EventGroup() : handle_(nullptr) {}

    explicit EventGroup(const StronglyTypedHandle strongly_handle) : handle_(strongly_handle.value) {}

    EventGroup& operator=(const EventGroup&) = delete;
    EventGroup(const EventGroup&) = delete;

    EventGroup& operator=(const StronglyTypedHandle strongly_handle)
    {
        if (handle_ != nullptr) {
            EMDEVIF_FATAL_HANDLER("Should not create event group on non-deleted event group!");
            return *this;
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    explicit EventGroup(const Attribute& attribute) : EventGroup(create(attribute)) {}

    EventGroup(EventGroup&& other) noexcept : handle_(other.handle_)
    {
        other.handle_ = nullptr;
    }

    EventGroup& operator=(EventGroup&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        this->handle_ = other.handle_;
        other.handle_ = nullptr;

        return *this;
    }

    ~EventGroup();

private:
    Handle handle_;
};

}  // namespace emdevif
