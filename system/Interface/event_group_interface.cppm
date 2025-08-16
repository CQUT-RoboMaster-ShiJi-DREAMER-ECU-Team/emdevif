/**
 * @file event_group_interface.cppm
 * @module emdevif.sys.event_group:interface
 * @author DuYicheng
 * @date 2025-08-16
 * @brief 事件组接口
 */

module;

#include <cstddef>

#include "emdevif/fault_handler.hpp"

export module emdevif.sys.event_group:interface;

export import emdevif.error_handler;
export import emdevif.sys.thread;

export namespace emdevif {

class EventGroup
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

    static void destroy(EventGroup& obj);

    void destroy()
    {
        destroy(*this);
        handle_ = nullptr;
    }

    static consteval auto maxEventBits() noexcept;

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
            EMDEVIF_FAULT_HANDLER("Should not create event group on non-deleted event group!");
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
