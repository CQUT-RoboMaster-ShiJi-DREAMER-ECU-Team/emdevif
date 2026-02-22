/**
 * @file event_group_interface.cppm
 * @brief 事件组接口
 */

// ReSharper disable CppDoxygenUnresolvedReference

module;

#include "emdevif/core/fatal_handler.h"

#include <cstdint>

export module emdevif.sys.eventGroup:interface;
import :definitions;

export import emdevif.core.error_handler;
export import emdevif.sys.thread;
export import emdevif.util.bitInt;
import emdevif.sys.thread;

export namespace emdevif {

class EventGroup
{
public:
    using Handle = void*;  ///< 底层实现句柄类型

    /**
     * 静态实例
     * @copydoc sys_static_instance
     */
    class StaticInstance;

    /// 属性
    struct Attribute {
        const char* name{};                        ///< 名称

        StaticInstance* static_instance{nullptr};  ///< 静态实例内存
    };

private:
    /// 强类型句柄
    /// @copydoc sys_strongly_typed_handle
    struct StronglyTypedHandle {
        Handle value;
    };

public:
    /**
     * 创建事件组
     * @param attribute 属性
     * @return 强类型句柄
     */
    static StronglyTypedHandle create(const Attribute& attribute);

    /**
     * 销毁事件组
     * @param obj 待销毁的事件组
     */
    static void destroy(EventGroup& obj);

    /**
     * 销毁事件组（自身）
     */
    void destroy()
    {
        destroy(*this);
        handle_ = nullptr;
    }

    /// 事件组的位数最大值
    static constexpr BitsType_t maxEventBits = detail::maxEventBits();

    /// 事件组的位类型，用于表示事件组的位
    using EventBits_t = UBitInt<maxEventBits>;

    /**
     * 获取事件位
     * @param in_isr 是否处于中断上下文
     * @return 一个 @ref maxEventBits 位的 UbitInt 类型数据，用以表示当前的事件组
     */
    auto getBits(bool in_isr) noexcept -> EventBits_t;

    /**
     * 置位事件位
     * @param in_isr 是否处于中断上下文
     * @param bits 待置位的事件位
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::OperationFail 失败
     */
    ErrorCode setBits(bool in_isr, EventBits_t bits) noexcept;

    /**
     * 清除事件位
     * @param in_isr 是否处于中断上下文
     * @param bits 待清除的事件位
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::OperationFail 失败
     */
    ErrorCode clearBits(bool in_isr, EventBits_t bits) noexcept;

    /**
     * 等待事件位
     * @param bits_wait_for 要等待的事件位
     * @param clear_on_exit 是否在返回时清理事件位
     * @param wait_for_all_bits 是否要等待所有事件位
     * @param timeout_tick 超时时间，默认无限等待
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::Timeout 等待超时
     */
    ErrorCode waitBits(EventBits_t bits_wait_for,
                       bool clear_on_exit,
                       bool wait_for_all_bits,
                       SysTick_t timeout_tick = Thread::maxDelay()) noexcept;

    /**
     * 同步事件位
     *
     * 调用此函数时，先置位事件位，再等待目标事件位全部被置位。
     * @param bits_to_set 要置位的事件位
     * @param bits_wait_for 要等待的目标事件位（每一位都要等待）
     * @param timeout_tick 超时时间，默认无限等待
     * @retval ErrorCode::Success 成功
     * @retval ErrorCode::Timeout 等待超时
     */
    ErrorCode sync(EventBits_t bits_to_set,
                   EventBits_t bits_wait_for,
                   SysTick_t timeout_tick = Thread::maxDelay()) noexcept;

    /**
     * 获取底层实现句柄
     * @return 底层实现句柄
     */
    [[nodiscard]] Handle getHandle() const
    {
        return handle_;
    }

    EventGroup() noexcept : handle_(nullptr) {}

    explicit EventGroup(const StronglyTypedHandle strongly_handle) noexcept : handle_(strongly_handle.value) {}

    EventGroup& operator=(const EventGroup&) = delete;
    EventGroup(const EventGroup&) = delete;

    EventGroup& operator=(const StronglyTypedHandle strongly_handle) noexcept
    {
        if (handle_ != nullptr) {
            EMDEVIF_FATAL_HANDLER("Should not create event group on non-deleted event group!");
            return *this;
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    explicit EventGroup(const Attribute& attribute) noexcept : EventGroup(create(attribute)) {}

    EventGroup(EventGroup&& other) noexcept : handle_(other.handle_)
    {
        other.handle_ = nullptr;
    }

    EventGroup& operator=(EventGroup&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        this->handle_ = other.handle_;
        other.handle_ = nullptr;

        return *this;
    }

    ~EventGroup();

private:
    Handle handle_;  ///< 底层实现句柄
};

}  // namespace emdevif
