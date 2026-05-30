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

/**
 * @brief 消息槽接口（CRTP 模式，由实现类提供具体操作，容量固定为 1）
 * @tparam Impl 实现类的模板模板参数
 * @tparam Type 消息元素类型
 */
template<template<typename T> class Impl, typename Type>
class MessageSlotInterface
{
public:
    using ImplType = Impl<Type>;
    using ValueType = Type;

    /// 消息槽容量（固定为 1）
    static constexpr std::size_t item_size = 1;

    /**
     * 强制推送数据到消息槽
     * @param in_isr 是否在中断上下文中调用
     * @param data 待推送的数据
     * @return 错误码
     */
    ErrorCode forcePush(bool in_isr, const Type& data)
    {
        return static_cast<ImplType*>(this)->forcePushImpl(in_isr, data);
    }

    /**
     * 查看消息槽中的数据（不移除）
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
     * 清空消息槽
     */
    void clear()
    {
        static_cast<ImplType*>(this)->clearImpl();
    }

    /**
     * 获取消息槽中已存储的元素数量
     * @return 已存储的元素数量
     */
    [[nodiscard]] std::size_t storeCount() const
    {
        return static_cast<const ImplType*>(this)->storeCountImpl();
    }

    /**
     * 获取消息槽中剩余的空闲槽位数量
     * @return 剩余空闲槽位数量
     */
    [[nodiscard]] std::size_t remainCount() const
    {
        return static_cast<const ImplType*>(this)->remainCountImpl();
    }

    /**
     * 获取消息槽最大容量（固定为 1）
     * @return 消息槽最大容量
     */
    [[nodiscard]] static constexpr std::size_t maxItemCount()
    {
        return 1;
    }

    /**
     * 获取消息槽底层句柄
     * @return 消息槽句柄
     */
    [[nodiscard]] auto getHandle() const
    {
        return static_cast<const ImplType*>(this)->getHandleImpl();
    }

protected:
    MessageSlotInterface() = default;
    MessageSlotInterface(const MessageSlotInterface&) = default;
    ~MessageSlotInterface() = default;
};

/**
 * @brief 判断某类型是否为消息槽实现的类型特征（主模板为 false）
 */
template<class Impl>
struct IsMessageSlot : public std::false_type {
};

/**
 * @brief 判断某类型是否为消息槽实现的简化表达式
 */
template<class Impl>
constexpr bool IsMessageSlot_v = IsMessageSlot<Impl>::value;

/**
 * @brief 有效消息槽实现概念
 */
template<class Impl>
concept ValidMessageSlot = IsMessageSlot_v<Impl>;

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_DATA_CONTAINER_MESSAGE_SLOT_HPP
