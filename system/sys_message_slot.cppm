/**
 * @file sys_message_slot.cppm
 * @brief 系统消息槽 - 由操作系统提供的队列实现的消息槽
 */

module;

#include <utility>

export module emdevif.sys.sysMessageSlot;

import emdevif.sys.thread;
import emdevif.sys.sysQueue;
import emdevif.container.messageSlot;
import emdevif.container.messageQueue;

namespace emdevif {

export template<typename Type>
class SysMessageSlot;

template<typename Type>
class SysQueueForSlotAdapter : public SysQueue<Type, 1>
{
protected:
    using QueueImpl_ = SysQueue<Type, 1>;

    // ReSharper disable once CppNonExplicitConvertingConstructor
    template<typename T>
    SysQueueForSlotAdapter(const T& init_param) : QueueImpl_(init_param)  // NOLINT(*-explicit-constructor)
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

export template<typename Type>
class SysMessageSlot : public MessageSlotInterface<SysMessageSlot, Type>
{
public:
    friend class MessageSlotInterface<SysMessageSlot, Type>;

private:
    using QueueAdapter_ = SysQueueForSlotAdapter<Type>;
    using QueueImpl_ = QueueAdapter_::QueueImpl_;

    QueueAdapter_ queue_impl_{};

public:
    using Handle = typename QueueImpl_::Handle;  ///< 句柄类型

    static auto create(const QueueImpl_::Attribute& attribute)
    {
        return QueueImpl_::create(attribute);
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
    SysMessageSlot(const T& init_param) : queue_impl_(init_param)  // NOLINT(*-explicit-constructor)
    {
    }

    template<typename T>
    SysMessageSlot& operator=(T&& other) noexcept
    {
        queue_impl_.operator=(std::forward<T>(other));

        return *this;
    }
};

}  // namespace emdevif
