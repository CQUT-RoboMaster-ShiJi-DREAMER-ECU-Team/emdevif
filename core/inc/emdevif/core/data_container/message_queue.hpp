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

/**
 * @brief 超时时间类型（以 tick 为单位）
 */
EMDEVIF_MODULE_EXPORT using MessageQueueTimeout_t = std::uint_fast32_t;

/**
 * @brief 消息队列接口（CRTP 模式，由实现类提供具体操作）
 * @tparam Impl 实现类的模板模板参数
 * @tparam Type 消息元素类型
 * @tparam item_size_ 消息队列容量
 */
EMDEVIF_MODULE_EXPORT
template<template<typename T, std::size_t sz> class Impl, typename Type, std::size_t item_size_>
class MessageQueueInterface
{
public:
    using ImplType = Impl<Type, item_size_>;
    using ValueType = Type;

    /// 消息队列容量
    static constexpr std::size_t item_size = item_size_;

    /**
     * 推送数据到队列
     * @param in_isr 是否在中断上下文中调用
     * @param data 待推送的数据
     * @param timeout_tick 超时时间（tick 数），默认 0 表示不等待
     * @return 错误码
     */
    ErrorCode push(bool in_isr, const Type& data, MessageQueueTimeout_t timeout_tick = 0U)
    {
        return static_cast<ImplType*>(this)->pushImpl(in_isr, data, timeout_tick);
    }

    /**
     * 强制推送数据到队列（忽略队列满的情况）
     * @param in_isr 是否在中断上下文中调用
     * @param data 待推送的数据
     * @return 错误码
     */
    ErrorCode forcePush(bool in_isr, const Type& data)
    {
        return static_cast<ImplType*>(this)->forcePushImpl(in_isr, data);
    }

    /**
     * 从队列弹出数据
     * @param in_isr 是否在中断上下文中调用
     * @param[out] data 接收弹出的数据
     * @param timeout_tick 超时时间（tick 数），默认 0 表示不等待
     * @return 错误码
     */
    ErrorCode pop(bool in_isr, Type& data, MessageQueueTimeout_t timeout_tick = 0U)
    {
        return static_cast<ImplType*>(this)->popImpl(in_isr, data, timeout_tick);
    }

    /**
     * 从队列弹出数据（不获取数据内容，只移除元素）
     * @param in_isr 是否在中断上下文中调用
     * @return 错误码
     */
    ErrorCode pop(bool in_isr)
    {
        return static_cast<ImplType*>(this)->popImpl(in_isr);
    }

    /**
     * 查看队列头部数据（不移除）
     * @param in_isr 是否在中断上下文中调用
     * @param[out] data 接收查看的数据
     * @param timeout_tick 超时时间（tick 数），默认 0 表示不等待
     * @return 错误码
     */
    ErrorCode peek(bool in_isr, Type& data, MessageQueueTimeout_t timeout_tick = 0U)
    {
        return static_cast<ImplType*>(this)->peekImpl(in_isr, data, timeout_tick);
    }

    /**
     * 清空队列
     */
    void clear()
    {
        static_cast<ImplType*>(this)->clearImpl();
    }

    /**
     * 获取队列中已存储的元素数量
     * @return 已存储的元素数量
     */
    [[nodiscard]] std::size_t storeCount() const
    {
        return static_cast<const ImplType*>(this)->storeCountImpl();
    }

    /**
     * 获取队列中剩余的空闲槽位数量
     * @return 剩余空闲槽位数量
     */
    [[nodiscard]] std::size_t remainCount() const
    {
        return static_cast<const ImplType*>(this)->remainCountImpl();
    }

    /**
     * 获取队列最大容量
     * @return 队列最大容量
     */
    [[nodiscard]] static constexpr std::size_t maxItemCount()
    {
        return item_size_;
    }

    /**
     * 获取队列底层句柄（用于 RTOS API 操作）
     * @return 队列句柄
     */
    [[nodiscard]] void* getHandle() const
    {
        return static_cast<const ImplType*>(this)->getHandleImpl();
    }

protected:
    MessageQueueInterface() = default;
    MessageQueueInterface(const MessageQueueInterface&) = default;
    ~MessageQueueInterface() = default;
};

/**
 * @brief 判断某类型是否为消息队列实现的类型特征（主模板为 false）
 */
EMDEVIF_MODULE_EXPORT template<class Impl>
struct IsMessageQueue : public std::false_type {
};

/**
 * @brief 判断某类型是否为消息队列实现的简化表达式
 */
EMDEVIF_MODULE_EXPORT template<class Impl>
constexpr bool IsMessageQueue_v = IsMessageQueue<Impl>::value;

/**
 * @brief 有效消息队列实现概念
 */
EMDEVIF_MODULE_EXPORT template<class Impl>
concept ValidMessageQueue = IsMessageQueue_v<Impl>;

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_DATA_CONTAINER_MESSAGE_QUEUE_HPP
