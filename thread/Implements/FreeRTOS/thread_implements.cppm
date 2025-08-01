/**
 * @file thread_implements.cppm
 * @module emdevif.thread:implements
 * @author 杜以成
 * @date 2025-06-18
 * @brief FreeRTOS 线程相关常数实现
 */

module;

#include "FreeRTOS.h"
#include "task.h"

#include "emdevif/fault_handler.hpp"

export module emdevif.thread:implements;
import :interface;

import emdevif.error_handler;

export namespace emdevif {

consteval auto Thread::MAX_DELAY() noexcept
{
    return portMAX_DELAY;
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

inline void Thread::delay(const uint32_t ticks)
{
    vTaskDelay(ticks);
}

inline void Thread::delayMs(const uint32_t ms)
{
    delay(pdMS_TO_TICKS(ms));
}

inline ErrorCode Thread::delayUntil(const uint32_t ticks)
{
    auto tcnt = xTaskGetTickCount();

    const TickType_t delay = TickType_t(ticks) - tcnt;

    if ((delay != 0U) && (0 == (delay >> (8 * sizeof(TickType_t) - 1)))) {
        vTaskDelayUntil(&tcnt, delay);
    }
    else {
        return ErrorCode::InvalidArgument;
    }

    return ErrorCode::Success;
}

Thread::StronglyTypedHandle Thread::create(const ThreadEntry entry, void* arguments, const Attribute& attribute)
{
    TaskHandle_t handle = nullptr;

    if (attribute.cb_mem != nullptr && attribute.stack_mem != nullptr && attribute.cb_size != 0U) {
        handle = xTaskCreateStatic(entry,
                                   attribute.name,
                                   attribute.stack_size,
                                   arguments,
                                   attribute.priority,
                                   static_cast<StackType_t*>(attribute.stack_mem),
                                   static_cast<StaticTask_t*>(attribute.cb_mem));
    }
    else {
        const auto result =
            xTaskCreate(entry, attribute.name, attribute.stack_size, arguments, attribute.priority, &handle);
        if (result != pdPASS) {
            handle = nullptr;
        }
    }

    return {.value = handle};
}

inline ErrorCode Thread::destroy(Handle handle)
{
    if (handle == nullptr) {
        exit();
    }

    const auto handle_value = static_cast<TaskHandle_t>(handle);

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

}  // namespace emdevif
