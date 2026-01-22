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

export template<template<typename T, std::size_t sz> class Impl, typename Type, std::size_t item_size>
class MessageQueueInterface
{
public:
    using ImplType = Impl<Type, item_size>;

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
        return item_size;
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

namespace detail {

template<template<typename T, std::size_t sz> class Impl, typename Type, std::size_t item_size>
class IsMessageQueueHelper
{
private:
    using ImplType_ = Impl<Type, item_size>;
    using InterfaceType_ = MessageQueueInterface<Impl, Type, item_size>;

public:
    static constexpr bool value =
        std::is_base_of_v<InterfaceType_, ImplType_> && std::is_convertible_v<ImplType_*, InterfaceType_*>;
};

}  // namespace detail

export template<template<typename T, std::size_t sz> class Impl, typename Type, std::size_t item_size>
struct IsMessageQueue : public std::false_type {
};

export template<template<typename T, std::size_t sz> class Impl, typename Type, std::size_t item_size>
constexpr bool IsMessageQueue_v = IsMessageQueue<Impl, Type, item_size>::value;

export template<template<typename T, std::size_t sz> class Impl, typename Type, std::size_t item_size>
concept ValidMessageQueue = IsMessageQueue_v<Impl, Type, item_size>;

}  // namespace emdevif
