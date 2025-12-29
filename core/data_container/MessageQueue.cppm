/**
 * @file MessageQueue.cppm
 * @brief 消息队列接口
 */

module;

#include <cstdint>

#include <type_traits>
#include <utility>

export module emdevif.container.messageQueue;

import emdevif.errorHandler;

export namespace emdevif {

using MessageQueueTimeout_t = std::uint_fast32_t;

template<template<typename T, std::size_t sz> class Impl, typename Type, std::size_t item_size>
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

template<template<typename T, std::size_t sz> class Impl, typename Type, std::size_t item_size>
class MessageQueue : public Impl<Type, item_size>
{
public:
    static_assert(std::is_base_of_v<MessageQueueInterface<Impl, Type, item_size>, Impl<Type, item_size>>,
                  "The template parameter `Impl\' should be derived from `MessageQueueInterface\'.");

    // 以下函数仅用于适配 Impl 的工厂函数（静态的 create 方法）

    constexpr MessageQueue() = default;

    // ReSharper disable once CppNonExplicitConvertingConstructor
    template<typename T>
    constexpr MessageQueue(const T& init_attribute) noexcept  // NOLINT(*-explicit-constructor)
        : Impl<Type, item_size>(init_attribute)
    {
    }

    template<typename T>
    constexpr MessageQueue& operator=(T&& other) noexcept
    {
        auto super = static_cast<Impl<Type, item_size>*>(this);
        super->operator=(std::forward<T>(other));

        return *this;
    }
};

}  // namespace emdevif
