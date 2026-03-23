/**
 * @file message_slot.hpp
 * @brief 消息槽
 */

#pragma once
#ifndef EMDEVIF_CORE_DATA_CONTAINER_MESSAGE_SLOT_HPP
    #define EMDEVIF_CORE_DATA_CONTAINER_MESSAGE_SLOT_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/core/error_handler.hpp"
        #include "emdevif/core/data_container/message_queue.hpp"

        #include <cstddef>

        #include <type_traits>
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

template<template<typename T> class Impl, typename Type>
class MessageSlotInterface
{
public:
    using ImplType = Impl<Type>;
    using ValueType = Type;

    static constexpr std::size_t item_size = 1;

    ErrorCode forcePush(bool in_isr, const Type& data)
    {
        return static_cast<ImplType*>(this)->forcePushImpl(in_isr, data);
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
        return 1;
    }

    [[nodiscard]] auto getHandle() const
    {
        return static_cast<const ImplType*>(this)->getHandleImpl();
    }

protected:
    MessageSlotInterface() = default;
    MessageSlotInterface(const MessageSlotInterface&) = default;
    ~MessageSlotInterface() = default;
};

template<class Impl>
struct IsMessageSlot : public std::false_type {
};

template<class Impl>
constexpr bool IsMessageSlot_v = IsMessageSlot<Impl>::value;

template<class Impl>
concept ValidMessageSlot = IsMessageSlot_v<Impl>;

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_DATA_CONTAINER_MESSAGE_SLOT_HPP
