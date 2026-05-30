/**
 * @file sys_queue.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_SYSTEM_SYS_QUEUE_HPP
#define EMDEVIF_SYSTEM_SYS_QUEUE_HPP

#include "emdevif/core/detail/config.hpp"

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#include "emdevif/core/fatal_handler.h"

#include "emdevif/core/error_handler.hpp"
#include "emdevif/core/data_container/message_queue.hpp"
#include "emdevif/system/thread.hpp"

#include <cstdint>

#include <type_traits>
#endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

/**
 * 系统队列
 * @tparam Type 队列每个元素的类型
 * @tparam item_size 队列的大小
 */
template<typename Type, std::size_t item_size>
class SysQueue : public MessageQueueInterface<SysQueue, Type, item_size>
{
public:
    friend class MessageQueueInterface<SysQueue, Type, item_size>;

    using Handle = void*;  ///< 句柄

    /**
     * @brief 系统队列的静态实例
     * @copydoc sys_static_instance
     */
    class StaticInstance;

    /// 属性
    struct Attribute {
        const char* name{};                        ///< 名称

        StaticInstance* static_instance{nullptr};  ///< 静态实例内存
    };

private:
    /**
     * @brief 强类型句柄，防止直接使用 void* 类型
     */
    struct StronglyTypedHandle {
        Handle value;
    };

public:  // todo 等消息队列接口做好了之后再来补注释
    static auto create(const Attribute& attribute) -> StronglyTypedHandle;

    static void destroy(SysQueue& obj);

    void destroy()
    {
        destroy(*this);
        handle_ = nullptr;
    }

private:
    /**
     * @brief 向队列尾部推送数据
     * @param in_isr 是否在中断上下文中调用
     * @param data 待推送的数据
     * @param timeout_tick 超时 tick 数，0 表示不等待
     * @retval ErrorCode::Success 推送成功
     * @retval ErrorCode::Timeout 超时
     */
    ErrorCode pushImpl(bool in_isr, const Type& data, SysTick_t timeout_tick = 0U);

    /**
     * @brief 强制向队列尾部推送数据（覆盖旧数据）
     * @param in_isr 是否在中断上下文中调用
     * @param data 待推送的数据
     * @retval ErrorCode::Success 推送成功
     * @retval ErrorCode::OperationFail 操作失败
     */
    ErrorCode forcePushImpl(bool in_isr, const Type& data);

    /**
     * @brief 从队列头部取出数据
     * @param[out] data 用于接收取出的数据
     * @param in_isr 是否在中断上下文中调用
     * @param timeout_tick 超时 tick 数，0 表示不等待
     * @retval ErrorCode::Success 取出成功
     * @retval ErrorCode::Timeout 超时
     */
    ErrorCode popImpl(bool in_isr, Type& data, SysTick_t timeout_tick = 0U);

    /**
     * @brief 从队列头部取出数据（丢弃数据）
     * @param in_isr 是否在中断上下文中调用
     * @retval ErrorCode::Success 取出成功
     * @retval ErrorCode::Timeout 超时
     */
    ErrorCode popImpl(bool in_isr);

    /**
     * @brief 查看队列头部的数据但不移除
     * @param[out] data 用于接收头部数据
     * @param in_isr 是否在中断上下文中调用
     * @param timeout_tick 超时 tick 数，0 表示不等待
     * @retval ErrorCode::Success 查看成功
     * @retval ErrorCode::Timeout 超时
     */
    ErrorCode peekImpl(bool in_isr, Type& data, SysTick_t timeout_tick = 0U);

    /**
     * @brief 清空队列中的所有数据
     */
    void clearImpl();

    /**
     * @brief 获取队列中已存储的元素数量
     * @return 已存储的元素数量
     */
    [[nodiscard]] std::size_t storeCountImpl() const;

    /**
     * @brief 获取队列中剩余可用空间
     * @return 剩余可存储的元素数量
     */
    [[nodiscard]] std::size_t remainCountImpl() const;

    [[nodiscard]] Handle getHandleImpl() const
    {
        return handle_;
    }

public:
    SysQueue() : handle_(nullptr) {}

    // ReSharper disable once CppNonExplicitConvertingConstructor
    // NOLINTNEXTLINE(*-explicit-constructor)
    SysQueue(const StronglyTypedHandle& strongly_handle) : handle_(strongly_handle.value) {}

    SysQueue& operator=(const SysQueue&) = delete;
    SysQueue(const SysQueue&) = delete;

    SysQueue& operator=(const StronglyTypedHandle& strongly_handle)
    {
        if (handle_ != nullptr) {
            EMDEVIF_FATAL_HANDLER("Should not create message queue on non-deleted message queue!");
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    explicit SysQueue(const Attribute& attribute) : SysQueue(create(attribute)) {}

    SysQueue(SysQueue&& other) noexcept : handle_(other.handle_)
    {
        other.handle_ = nullptr;
    }

    SysQueue& operator=(SysQueue&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        this->handle_ = other.handle_;
        other.handle_ = nullptr;

        return *this;
    }

    ~SysQueue();

private:
    Handle handle_;  ///< 底层实现的句柄
};

template<typename Type, std::size_t item_size>
struct IsMessageQueue<SysQueue<Type, item_size>> : public std::true_type {
};

}  // namespace emdevif

#include "emdevif/system_impl/sys_queue.inl"

#endif  // !EMDEVIF_SYSTEM_SYS_QUEUE_HPP
