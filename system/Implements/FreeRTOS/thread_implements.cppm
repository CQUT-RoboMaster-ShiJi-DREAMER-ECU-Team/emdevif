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

template<std::size_t stack_depth>
class Thread::StaticInstance
{
public:
    static_assert(stack_depth > 0U, "Stack depth must be greater than 0.");

    StaticInstance() noexcept : instance(), stack_buffer() {}

    friend class Thread;

    explicit operator StaticTask_t&() noexcept
    {
        return instance;
    }

    explicit operator StackType_t&() noexcept
    {
        return *stack_buffer;
    }

private:
    StaticTask_t instance;
    StackType_t stack_buffer[stack_depth];
};

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

Thread::StronglyTypedHandle Thread::create(const Attribute& attribute,
                                           const ThreadEntry entry,
                                           void* arguments) noexcept
{
    TaskHandle_t handle = nullptr;

    if (attribute.static_instance != nullptr && attribute.stack_size != 0U) {
        // 由于此处的 Thread::StaticInstance 是指针类型，且它的自定义类型转换过程不涉及这个模板参数，
        // 因此它的模板参数的取值不影响结果。但不能为 0，因为这样做会构造出零长数组。
        auto& static_instance = *static_cast<Thread::StaticInstance<1>*>(attribute.static_instance);

        handle = xTaskCreateStatic(entry,
                                   attribute.name,
                                   attribute.stack_size,
                                   arguments,
                                   priorityMapToSystem(attribute.priority),
                                   &static_cast<StackType_t&>(static_instance),
                                   &static_cast<StaticTask_t&>(static_instance));
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
    if (obj.handle_ == nullptr) {
        return ErrorCode::InvalidArgument;
    }

    const auto handle_value = static_cast<TaskHandle_t>(obj.handle_);
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

inline void Thread::startScheduler()
{
    vTaskStartScheduler();
}

inline void Thread::endScheduler()
{
    vTaskEndScheduler();
}

inline void Thread::yield()
{
    taskYIELD();
}

[[maybe_unused]] inline void Thread::join() {}  // NOLINT

Thread::~Thread()
{
    if (handle_ != nullptr) {
        destroy(*this);
        handle_ = nullptr;
    }

    if (func_wrapper_memory_block_ != nullptr) {
        heap::destruct(func_wrapper_memory_block_);
    }
}

}  // namespace emdevif
