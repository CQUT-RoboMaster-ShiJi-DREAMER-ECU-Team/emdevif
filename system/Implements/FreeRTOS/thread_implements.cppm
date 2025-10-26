/**
 * @file thread_implements.cppm
 * @author 杜以成
 * @date 2025-06-18
 * @brief FreeRTOS 线程相关常数实现
 */

module;

#include "FreeRTOS.h"
#include "task.h"

#include <utility>
#include <type_traits>

#include "emdevif/fatal_handler.h"

#if (configMAX_PRIORITIES < 6)
#error \
    "emdevif thread requires the max priorities more than or equal to 6. Please change the value of macro `configMAX_PRIORITIES' in FreeRTOSConfig.h."
#endif

export module emdevif.sys.thread:implements;
import :interface;

import emdevif.errorHandler;

export namespace emdevif {

static_assert(std::is_same_v<SysTick_t, ::TickType_t>, "We need to keep SysTick_t same to TickType_t in FreeRTOS");

template<std::size_t stack_depth>
class Thread::StaticInstance
{
public:
    static_assert(stack_depth > 0U, "Stack depth must be greater than 0.");

    StaticInstance() noexcept : instance(), stack_buffer() {}

    friend class Thread;

private:
    explicit operator StaticTask_t&() noexcept
    {
        return instance;
    }

    explicit operator StackType_t&() noexcept
    {
        return *stack_buffer;
    }

public:
    /**
     * 获取静态实例的地址（用于初始化填入 Attribute 形参）
     * @return 静态实例存储的地址
     */
    void* getInstanceAddr() noexcept
    {
        return &instance;
    }

    /**
     * 获取静态实例的栈深（用于初始化填入 Attribute 形参）
     * @return 静态实例的栈深（以字为单位）
     */
    std::size_t getStackDepth() noexcept  // NOLINT
    {
        return stack_depth;
    }

private:
    StaticTask_t instance;                  ///< 任务静态实例
    StackType_t stack_buffer[stack_depth];  ///< 任务栈内存
};

consteval SysTick_t Thread::maxDelay() noexcept
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

constexpr auto Thread::priorityMapToSystem(const Priority priority) noexcept
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

inline SysTick_t Thread::getTick(const bool in_isr) noexcept
{
    if (in_isr) {
        return xTaskGetTickCount();
    }
    else {
        return xTaskGetTickCountFromISR();
    }
}

inline void Thread::delay(const SysTick_t ticks) noexcept
{
    vTaskDelay(ticks);
}

inline ErrorCode Thread::delayUntil(const SysTick_t ticks) noexcept
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

inline SysTick_t Thread::msToTick(const SysTick_t ms) noexcept
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

ErrorCode Thread::destroy(Thread& obj) noexcept
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

inline void Thread::exit() noexcept
{
    handle_ = nullptr;
    vTaskDelete(nullptr);

    // 程序不应当执行到此处
    EMDEVIF_FATAL_HANDLER("Should not running here.");
    while (true) {
    }
}

inline void Thread::suspend(Handle handle) noexcept
{
    vTaskSuspend(static_cast<TaskHandle_t>(handle));
}

inline void Thread::resume(bool in_isr, Handle handle) noexcept
{
    if (in_isr) {
        const auto xYieldRequired = xTaskResumeFromISR(static_cast<TaskHandle_t>(handle));

        portYIELD_FROM_ISR(xYieldRequired);
    }
    else {
        vTaskResume(static_cast<TaskHandle_t>(handle));
    }
}

inline void Thread::startScheduler() noexcept
{
    vTaskStartScheduler();
}

inline void Thread::endScheduler() noexcept
{
    vTaskEndScheduler();
}

inline void Thread::yield() noexcept
{
    taskYIELD();
}

[[maybe_unused]] inline bool Thread::joinable() noexcept  // NOLINT
{
    return false;
}

[[maybe_unused]] inline void Thread::join() noexcept  // NOLINT
{
    EMDEVIF_FATAL_HANDLER("FreeRTOS can not join!");
}

Thread::~Thread() noexcept
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
