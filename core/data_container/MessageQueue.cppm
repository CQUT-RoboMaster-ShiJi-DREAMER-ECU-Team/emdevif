/**
 * @file MessageQueue.cppm
 * @brief 消息队列接口
 */

module;

#include <cstdint>

#include <type_traits>

export module emdevif.container.messageQueue;

import emdevif.errorHandler;

namespace emdevif {

export using MessageQueueTimeout_t = std::uint_fast32_t;

export template<template<typename T, std::size_t sz> class Impl, typename Type, std::size_t item_size_>
class MessageQueueInterface
{
public:
    using ImplType = Impl<Type, item_size_>;
    using ValueType = Type;

    static constexpr std::size_t item_size = item_size_;

    ErrorCode push(bool in_isr, const Type& data, MessageQueueTimeout_t timeout_tick = 0U)
    {
        return static_cast<ImplType*>(this)->pushImpl(in_isr, data, timeout_tick);
    }

    ErrorCode forcePush(bool in_isr, const Type& data)
    {
        return static_cast<ImplType*>(this)->forcePushImpl(in_isr, data);
    }

    ErrorCode pop(bool in_isr, Type& data, MessageQueueTimeout_t timeout_tick = 0U)
    {
        return static_cast<ImplType*>(this)->popImpl(in_isr, data, timeout_tick);
    }

    ErrorCode pop(bool in_isr)
    {
        return static_cast<ImplType*>(this)->popImpl(in_isr);
    }

    ErrorCode peek(bool in_isr, Type& data, MessageQueueTimeout_t timeout_tick = 0U)
    {
        return static_cast<ImplType*>(this)->peekImpl(in_isr, data, timeout_tick);
    }

    void clear()
    {
        static_cast<ImplType*>(this)->clearImpl();
    }

    [[nodiscard]] std::size_t storeCount() const
    {
        return static_cast<const ImplType*>(this)->storeCountImpl();
    }

    [[nodiscard]] std::size_t remainCount() const
    {
        return static_cast<const ImplType*>(this)->remainCountImpl();
    }

    [[nodiscard]] static constexpr std::size_t maxItemCount()
    {
        return item_size_;
    }

    [[nodiscard]] void* getHandle() const
    {
        return static_cast<const ImplType*>(this)->getHandleImpl();
    }

protected:
    MessageQueueInterface() = default;
    MessageQueueInterface(const MessageQueueInterface&) = default;
    ~MessageQueueInterface() = default;
};

export template<class Impl>
struct IsMessageQueue : public std::false_type {
};

export template<class Impl>
constexpr bool IsMessageQueue_v = IsMessageQueue<Impl>::value;

export template<class Impl>
concept ValidMessageQueue = IsMessageQueue_v<Impl>;

}  // namespace emdevif
