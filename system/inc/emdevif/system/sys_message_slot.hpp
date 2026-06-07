/**
 * @file sys_message_slot.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_SYSTEM_SYS_MESSAGE_SLOT_HPP
    #define EMDEVIF_SYSTEM_SYS_MESSAGE_SLOT_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/core/error_handler.hpp"
        #include "emdevif/core/data_container/message_queue.hpp"
        #include "emdevif/core/data_container/message_slot.hpp"
        #include "emdevif/system/thread.hpp"
        #include "emdevif/system/sys_queue.hpp"

        #include <utility>
        #include <type_traits>
    #endif

namespace emdevif {

EMDEVIF_MODULE_EXPORT
template<typename Type>
class SysMessageSlot;

namespace detail {

/**
 * @brief SysMessageSlot 专用的队列适配器，屏蔽 push/pop 接口
 * @tparam Type 消息数据类型
 */
template<typename Type>
class SysQueueForSlotAdapter : public SysQueue<Type, 1>
{
protected:
    using QueueImpl_ = SysQueue<Type, 1>;

    // ReSharper disable once CppNonExplicitConvertingConstructor
    template<typename T>
    SysQueueForSlotAdapter(T&& init_param)  // NOLINT(*-explicit-constructor, *-forwarding-reference-overload)
        : QueueImpl_(std::forward<T>(init_param))
    {
    }

    template<typename T>
    SysQueueForSlotAdapter& operator=(T&& other) noexcept
    {
        auto super = static_cast<QueueImpl_*>(this);
        super->operator=(std::forward<T>(other));

        return *this;
    }

    SysQueueForSlotAdapter() = default;
    SysQueueForSlotAdapter(const SysQueueForSlotAdapter&) = default;
    ~SysQueueForSlotAdapter() = default;

public:
    ErrorCode push(bool in_isr, const Type& data, MessageQueueTimeout_t timeout_tick = 0U) = delete;
    ErrorCode pop(bool in_isr, Type& data, MessageQueueTimeout_t timeout_tick = 0U) = delete;
    ErrorCode pop(bool in_isr) = delete;

    ErrorCode pushImpl(bool in_isr, const Type& data, SysTick_t timeout_tick = 0U) = delete;
    ErrorCode popImpl(bool in_isr, Type& data, SysTick_t timeout_tick = 0U) = delete;
    ErrorCode popImpl(bool in_isr) = delete;

    friend class SysMessageSlot<Type>;
};

}  // namespace detail

EMDEVIF_MODULE_EXPORT_BEGIN

template<typename Type>
class SysMessageSlot : public MessageSlotInterface<SysMessageSlot, Type>
{
public:
    friend class MessageSlotInterface<SysMessageSlot, Type>;

private:
    using QueueAdapter_ = detail::SysQueueForSlotAdapter<Type>;
    using QueueImpl_ = QueueAdapter_::QueueImpl_;

    QueueAdapter_ queue_impl_{};

public:
    using Handle = QueueImpl_::Handle;  ///< 句柄类型

    /**
     * 通过 SysQueueBuilder 创建 SysMessageSlot
     * @param builder Builder
     * @return 创建好的 SysMessageSlot
     */
    static SysMessageSlot create(SysQueueBuilder builder)
    {
        SysMessageSlot slot;
        slot.queue_impl_ = QueueImpl_{std::move(builder)};
        return slot;
    }

    static void destroy(SysMessageSlot& obj)
    {
        obj.queue_impl_.destroy();
    }

    void destroy()
    {
        destroy(*this);
    }

private:
    ErrorCode forcePushImpl(bool in_isr, const Type& data)
    {
        return queue_impl_.forcePush(in_isr, data);
    }

    ErrorCode peekImpl(bool in_isr, Type& data, MessageQueueTimeout_t timeout_tick = 0U)
    {
        return queue_impl_.peek(in_isr, data, timeout_tick);
    }

    void clearImpl()
    {
        queue_impl_.clear();
    }

    [[nodiscard]] std::size_t storeCountImpl() const
    {
        return queue_impl_.storeCount();
    }

    [[nodiscard]] std::size_t remainCountImpl() const
    {
        return queue_impl_.remainCount();
    }

    [[nodiscard]] Handle getHandleImpl() const
    {
        return queue_impl_.getHandle();
    }

public:
    SysMessageSlot() = default;

    // ReSharper disable once CppNonExplicitConvertingConstructor
    template<typename T>
    SysMessageSlot(T&& init_param)  // NOLINT(*-explicit-constructor, *-forwarding-reference-overload)
        : queue_impl_(std::forward<T>(init_param))
    {
    }

    template<typename T>
    SysMessageSlot& operator=(T&& other) noexcept
    {
        queue_impl_.operator=(std::forward<T>(other));

        return *this;
    }
};

template<typename Type>
struct IsMessageSlot<SysMessageSlot<Type>> : public std::true_type {
};

EMDEVIF_MODULE_EXPORT_END

}  // namespace emdevif

#endif  // !EMDEVIF_SYSTEM_SYS_MESSAGE_SLOT_HPP
