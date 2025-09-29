/**
 * @file event_group_implements.cppm
 * @module emdevif.sys.event_group:implements
 * @author DuYicheng
 * @date 2025-08-16
 * @brief 事件组 FreeRTOS 实现
 */

// ReSharper disable CppMemberFunctionMayBeConst

module;

#include <cstdint>

// ReSharper disable once CppUnusedIncludeDirective
// 注意，这个头文件必须包含，因为 UBitInt 的构造函数依赖于 std::integral，
// 如果没有这个头文件，会导致模板推导失败
#include <concepts>

#include "FreeRTOS.h"
#include "event_groups.h"

export module emdevif.sys.eventGroup:implements;
import :definitions;
import :interface;

import emdevif.util.bitInt;

export namespace emdevif {

class EventGroup::StaticInstance
{
public:
    StaticInstance() noexcept : static_instance() {}

    explicit operator StaticEventGroup_t&()
    {
        return static_instance;
    }

private:
    StaticEventGroup_t static_instance;
};

EventGroup::StronglyTypedHandle EventGroup::create(const Attribute& attribute)
{
    Handle handle;

    if (attribute.static_instance != nullptr && attribute.instance_size != 0) {
        auto& static_instance = static_cast<StaticEventGroup_t&>(*attribute.static_instance);

        handle = xEventGroupCreateStatic(&static_instance);
    }
    else {
        handle = xEventGroupCreate();
    }

    return {handle};
}

void EventGroup::destroy(EventGroup& obj)
{
    if (obj.handle_ != nullptr) {
        vEventGroupDelete(static_cast<EventGroupHandle_t>(obj.handle_));

        obj.handle_ = nullptr;
    }
}

inline auto EventGroup::getBits(const bool in_isr) noexcept -> EventGroup::EventBits_t
{
    ::EventBits_t ret;

    if (in_isr) {
        ret = xEventGroupGetBitsFromISR(static_cast<EventGroupHandle_t>(handle_));
    }
    else {
        ret = xEventGroupGetBits(static_cast<EventGroupHandle_t>(handle_));
    }

    return EventGroup::EventBits_t(ret);
}

inline auto EventGroup::setBits(const bool in_isr, const EventBits_t bits) noexcept -> EventGroup::EventBits_t
{
    uint_fast32_t ret;

    if (in_isr) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        ret = xEventGroupSetBitsFromISR(static_cast<EventGroupHandle_t>(handle_), bits, &xHigherPriorityTaskWoken);

        if (ret == pdPASS) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    else {
        ret = xEventGroupSetBits(static_cast<EventGroupHandle_t>(handle_), bits);
    }

    return EventGroup::EventBits_t(ret);
}

inline auto EventGroup::clearBits(const bool in_isr, const EventBits_t bits) noexcept -> EventGroup::EventBits_t
{
    uint_fast32_t ret;

    if (in_isr) {
        ret = xEventGroupClearBitsFromISR(static_cast<EventGroupHandle_t>(handle_), bits);

        if (ret == pdTRUE) {
            portYIELD_FROM_ISR(pdTRUE);
        }
    }
    else {
        ret = xEventGroupClearBits(static_cast<EventGroupHandle_t>(handle_), bits);
    }

    return EventGroup::EventBits_t(ret);
}

inline auto EventGroup::waitBits(const EventBits_t bits_wait_for,
                                 const bool clear_on_exit,
                                 const bool wait_for_all_bits,
                                 const SysTick_t timeout_tick) noexcept -> EventGroup::EventBits_t
{
    return EventGroup::EventBits_t(xEventGroupWaitBits(static_cast<EventGroupHandle_t>(handle_),
                                                       bits_wait_for,
                                                       clear_on_exit,
                                                       wait_for_all_bits,
                                                       timeout_tick));
}

inline EventGroup::EventBits_t EventGroup::sync(const EventBits_t bits_to_set,
                                                const EventBits_t bits_wait_for,
                                                const SysTick_t timeout_tick) noexcept
{
    return EventGroup::EventBits_t(
        xEventGroupSync(static_cast<EventGroupHandle_t>(handle_), bits_to_set, bits_wait_for, timeout_tick));
}

EventGroup::~EventGroup()
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif
