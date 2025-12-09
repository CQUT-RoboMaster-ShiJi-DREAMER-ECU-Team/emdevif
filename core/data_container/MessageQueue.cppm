/**
 * @file MessageQueue.cppm
 * @brief 消息队列接口
 */

module;

#include <type_traits>
#include <utility>

export module emdevif.container.messageQueue;

import emdevif.errorHandler;

export namespace emdevif {

template<template<typename T, std::size_t sz> class Impl, typename Type, std::size_t item_size>
class MessageQueueInterface
{
public:
    using ImplType = Impl<Type, item_size>;

    ErrorCode push(bool in_isr, const Type& data, std::size_t timeout = 0U)
    {
        return static_cast<ImplType*>(this)->pushImpl(in_isr, data, timeout);
    }

    ErrorCode pop(bool in_isr, Type& data, std::size_t timeout = 0U)
    {
        return static_cast<ImplType*>(this)->popImpl(in_isr, data, timeout);
    }

    ErrorCode pop(bool in_isr)
    {
        return static_cast<ImplType*>(this)->popImpl(in_isr);
    }

    ErrorCode peek(bool in_isr, Type& data, std::size_t timeout = 0U)
    {
        return static_cast<ImplType*>(this)->peekImpl(in_isr, data, timeout);
    }

    [[nodiscard]] std::size_t storeCount()
    {
        return static_cast<ImplType*>(this)->storeCountImpl();
    }
    [[nodiscard]] std::size_t storeCount() const
    {
        return static_cast<const ImplType*>(this)->storeCountImpl();
    }

    [[nodiscard]] std::size_t remainCount()
    {
        return static_cast<ImplType*>(this)->remainCountImpl();
    }
    [[nodiscard]] std::size_t remainCount() const
    {
        return static_cast<const ImplType*>(this)->remainCountImpl();
    }

    [[nodiscard]] void* getHandle()
    {
        return static_cast<ImplType*>(this)->getHandleImpl();
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
    // 以下函数仅用于适配 Impl 的工厂函数（静态的 create 方法）

    constexpr MessageQueue() = default;

    template<typename T>
    constexpr MessageQueue(const T& other) noexcept  // NOLINT(*-explicit-constructor)
        : Impl<Type, item_size>(other)
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
