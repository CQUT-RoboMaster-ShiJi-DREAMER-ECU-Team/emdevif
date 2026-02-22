/**
 * @file message_queue.hpp
 * @brief 消息队列接口
 */

#pragma once
#ifndef EMDEVIF_CORE_DATA_CONTAINER_MESSAGE_QUEUE_HPP
#define EMDEVIF_CORE_DATA_CONTAINER_MESSAGE_QUEUE_HPP

#include "emdevif/core/detail/config.hpp"

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#include "emdevif/core/error_handler.hpp"

#include <cstdint>

#include <type_traits>
#endif

namespace emdevif {

EMDEVIF_MODULE_EXPORT using MessageQueueTimeout_t = std::uint_fast32_t;

EMDEVIF_MODULE_EXPORT
template<template<typename T, std::size_t sz> class Impl, typename Type, std::size_t item_size_>
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

EMDEVIF_MODULE_EXPORT template<class Impl>
struct IsMessageQueue : public std::false_type {
};

EMDEVIF_MODULE_EXPORT template<class Impl>
constexpr bool IsMessageQueue_v = IsMessageQueue<Impl>::value;

EMDEVIF_MODULE_EXPORT template<class Impl>
concept ValidMessageQueue = IsMessageQueue_v<Impl>;

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_DATA_CONTAINER_MESSAGE_QUEUE_HPP
