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

auto Thread::getTick(const bool in_isr)
{
    if (in_isr) {
        return xTaskGetTickCount();
    }
    else {
        return xTaskGetTickCountFromISR();
    }
}

void Thread::delay(const uint32_t ticks)
{
    vTaskDelay(ticks);
}

void Thread::delayMs(const uint32_t ms)
{
    delay(pdMS_TO_TICKS(ms));
}

ErrorCode Thread::delayUntil(const uint32_t ticks)
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

Thread::Handle Thread::create(const ThreadEntry entry, void* arguments, const Attribute& attribute)
{
    xTaskHandle handle = nullptr;

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

    return handle;
}

void Thread::destroy(const Handle handle)
{
    if (handle == nullptr) {
        exit();
    }

    const auto handle_value = static_cast<TaskHandle_t>(handle);

    if (eTaskGetState(handle_value) != eDeleted) {
        vTaskDelete(handle_value);
    }
    else {
        EMDEVIF_FAULT_HANDLER("Delete deleted task!");
    }
}

void Thread::exit()
{
    vTaskDelete(nullptr);
}

}  // namespace emdevif
