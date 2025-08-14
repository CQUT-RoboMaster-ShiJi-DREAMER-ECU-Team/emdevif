/**
 * @file thread_implements.cppm
 * @module emdevif.sys.thread:implements
 * @author 杜以成
 * @date 2025-06-18
 * @brief FreeRTOS 线程相关常数实现
 */

module;

#include "FreeRTOS.h"
#include "task.h"

#include <utility>

#include "emdevif/fault_handler.hpp"

#if (configMAX_PRIORITIES < 6)
#error \
    "emdevif thread requires the max priorities more than or equal to 6. Please change the value of macro `configMAX_PRIORITIES' in FreeRTOSConfig.h."
#endif

export module emdevif.sys.thread:implements;
import :interface;

import emdevif.error_handler;

export namespace emdevif {

consteval auto Thread::MAX_DELAY() noexcept
{
    return portMAX_DELAY;
}

consteval auto Thread::priorityMapRange() noexcept
{
    constexpr UBaseType_t max_priority = configMAX_PRIORITIES - 1;
    constexpr UBaseType_t centered_priority = (max_priority - static_cast<UBaseType_t>(Priority::Realtime)) / 2U;

    return std::pair<UBaseType_t, UBaseType_t>{centered_priority,
                                               centered_priority + static_cast<UBaseType_t>(Priority::Realtime)};
}

auto Thread::priorityMapToSystem(const Priority priority)
{
    if (priority == Priority::Idle) {
        return static_cast<UBaseType_t>(0U);
    }

    constexpr UBaseType_t max_priority = configMAX_PRIORITIES - 1;

    if (priority == Priority::Max) {
        return max_priority;
    }

    const auto [centered_priority, map_max_priority] = priorityMapRange();
    return static_cast<UBaseType_t>(centered_priority + static_cast<UBaseType_t>(priority));
}

inline auto Thread::getTick(const bool in_isr)
{
    if (in_isr) {
        return xTaskGetTickCount();
    }
    else {
        return xTaskGetTickCountFromISR();
    }
}

inline void Thread::delay(const SysTick_t ticks)
{
    vTaskDelay(ticks);
}

inline ErrorCode Thread::delayUntil(const SysTick_t ticks)
{
    auto tcnt = xTaskGetTickCount();

    const TickType_t delay = static_cast<TickType_t>(ticks) - tcnt;

    if ((delay != 0U) && (0 == (delay >> (8 * sizeof(TickType_t) - 1)))) {
        vTaskDelayUntil(&tcnt, delay);
    }
    else {
        return ErrorCode::InvalidArgument;
    }

    return ErrorCode::Success;
}

inline auto Thread::msToTick(const SysTick_t ms)
{
    return pdMS_TO_TICKS(ms);
}

Thread::StronglyTypedHandle Thread::create(const Attribute& attribute, const ThreadEntry entry, void* arguments)
{
    TaskHandle_t handle = nullptr;

    if (attribute.cb_mem != nullptr && attribute.stack_mem != nullptr && attribute.cb_size != 0U) {
        handle = xTaskCreateStatic(entry,
                                   attribute.name,
                                   attribute.stack_size,
                                   arguments,
                                   priorityMapToSystem(attribute.priority),
                                   static_cast<StackType_t*>(attribute.stack_mem),
                                   static_cast<StaticTask_t*>(attribute.cb_mem));
    }
    else {
        const auto result = xTaskCreate(entry,
                                        attribute.name,
                                        attribute.stack_size,
                                        arguments,
                                        priorityMapToSystem(attribute.priority),
                                        &handle);
        if (result != pdPASS) {
            handle = nullptr;
        }
    }

    return {.value = handle};
}

ErrorCode Thread::destroy(Thread& obj)
{
    if (obj.getHandle() == nullptr) {
        return ErrorCode::InvalidArgument;
    }

    const auto handle_value = static_cast<TaskHandle_t>(obj.getHandle());
    obj.handle_ = nullptr;

    if (eTaskGetState(handle_value) != eDeleted) {
        vTaskDelete(handle_value);
    }
    else {
        return ErrorCode::InvalidArgument;
    }

    return ErrorCode::Success;
}

inline void Thread::exit()
{
    handle_ = nullptr;
    vTaskDelete(nullptr);

    // 程序不应当执行到此处
    EMDEVIF_FAULT_HANDLER("Should not running here.");
    while (true) {
    }
}

inline void Thread::suspend(Handle handle)
{
    vTaskSuspend(static_cast<TaskHandle_t>(handle));
}

inline void Thread::resume(bool in_isr, Handle handle)
{
    if (in_isr) {
        const auto xYieldRequired = xTaskResumeFromISR(static_cast<TaskHandle_t>(handle));

        portYIELD_FROM_ISR(xYieldRequired);
    }
    else {
        vTaskResume(static_cast<TaskHandle_t>(handle));
    }
}

inline void Thread::yield()
{
    portYIELD();
}

[[maybe_unused]] inline void Thread::join() {}  // NOLINT

Thread::~Thread()
{
    if (handle_ != nullptr) {
        destroy(*this);
    }
}

}  // namespace emdevif
