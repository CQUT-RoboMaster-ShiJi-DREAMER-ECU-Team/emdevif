/**
 * @file semaphore_implements.cppm
 * @module emdevif.sys.semaphore:implements
 * @author DuYicheng
 * @date 2025-08-16
 * @brief 信号量 FreeRTOS 实现
 */

// ReSharper disable CppMemberFunctionMayBeConst

module;

#include <cstddef>

#include "FreeRTOS.h"
#include "semphr.h"

#include "emdevif/fault_handler.hpp"

export module emdevif.sys.semaphore:implements;
import :interface;

export import emdevif.error_handler;

// for template<std::ptrdiff_t least_max_value> CountingSemaphore
export namespace emdevif {

template<std::ptrdiff_t least_max_value>
class CountingSemaphore<least_max_value>::StaticInstance
{
public:
    StaticInstance() noexcept : instance() {}

    explicit operator StaticSemaphore_t&() noexcept
    {
        return instance;
    }

private:
    StaticSemaphore_t instance;
};

template<std::ptrdiff_t least_max_value>
auto CountingSemaphore<least_max_value>::create(const Attribute& attribute)
    -> CountingSemaphore<least_max_value>::StronglyTypedHandle
{
    Handle handle = nullptr;

    if (attribute.static_instance != nullptr && attribute.instance_size != 0U) {
        auto& static_instance =
            *static_cast<CountingSemaphore<least_max_value>::StaticInstance*>(attribute.static_instance);

        handle = xSemaphoreCreateCountingStatic(least_max_value, 0U, &static_cast<StaticSemaphore_t&>(static_instance));
    }
    else {
        handle = xSemaphoreCreateCounting(least_max_value, 0U);
    }

    return {handle};
}

template<std::ptrdiff_t least_max_value>
void CountingSemaphore<least_max_value>::destroy(CountingSemaphore& obj)
{
    if (obj.handle_ != nullptr) {
        vSemaphoreDelete(obj.handle_);
        obj.handle_ = nullptr;
    }
}

template<std::ptrdiff_t least_max_value>
void CountingSemaphore<least_max_value>::release(bool in_isr) noexcept
{
    if (in_isr) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        xSemaphoreGiveFromISR(handle_, &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else {
        xSemaphoreGive(handle_);
    }
}

template<std::ptrdiff_t least_max_value>
ErrorCode CountingSemaphore<least_max_value>::acquire(bool in_isr, SysTick_t timeout_tick) noexcept
{
    if (in_isr) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        auto ret = xSemaphoreTakeFromISR(handle_, &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        return (ret == pdTRUE ? ErrorCode::Success : ErrorCode::OperationFail);
    }
    else {
        auto ret = xSemaphoreTake(static_cast<SemaphoreHandle_t>(handle_), timeout_tick);

        return (ret == pdTRUE ? ErrorCode::Success : ErrorCode::Timeout);
    }
}

template<std::ptrdiff_t least_max_value>
CountingSemaphore<least_max_value>::~CountingSemaphore()
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif

// for template<> CountingSemaphore<1>
export namespace emdevif {

template<>
class CountingSemaphore<1>
{
public:
    using Handle = void*;

    class StaticInstance;

    struct Attribute {
        const char* name{};                        ///< 名称

        StaticInstance* static_instance{nullptr};  ///< 静态实例内存
        std::size_t instance_size{0U};             ///< 实例大小
    };

private:
    struct StronglyTypedHandle {
        Handle value;
    };

public:
    static StronglyTypedHandle create(const Attribute& attribute);

    static void destroy(CountingSemaphore& obj);

    void destroy()
    {
        destroy(*this);
        handle_ = nullptr;
    }

    static constexpr std::ptrdiff_t max = 1;

    void release(bool in_isr) noexcept;

    ErrorCode acquire(bool in_isr, SysTick_t timeout_tick = Thread::MAX_DELAY()) noexcept;

    ErrorCode try_acquire(const bool in_isr) noexcept
    {
        return acquire(in_isr, 0U);
    }

    [[nodiscard]] Handle getHandle() const
    {
        return handle_;
    }

    CountingSemaphore() : handle_(nullptr) {}

    explicit CountingSemaphore(const StronglyTypedHandle strongly_handle) : handle_(strongly_handle.value) {}

    CountingSemaphore& operator=(const CountingSemaphore&) = delete;
    CountingSemaphore(const CountingSemaphore&) = delete;

    CountingSemaphore& operator=(const StronglyTypedHandle strongly_handle)
    {
        if (handle_ != nullptr) {
            EMDEVIF_FAULT_HANDLER("Should not create binary semaphore on non-deleted binary semaphore!");
            return *this;
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    explicit CountingSemaphore(const Attribute& attribute) : CountingSemaphore(create(attribute)) {}

    CountingSemaphore(CountingSemaphore&& other) noexcept : handle_(other.handle_)
    {
        other.handle_ = nullptr;
    }

    CountingSemaphore& operator=(CountingSemaphore&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        this->handle_ = other.handle_;
        other.handle_ = nullptr;

        return *this;
    }

    ~CountingSemaphore();

private:
    Handle handle_;
};

class CountingSemaphore<1>::StaticInstance
{
public:
    StaticInstance() noexcept : instance() {}

    explicit operator StaticSemaphore_t&() noexcept
    {
        return instance;
    }

private:
    StaticSemaphore_t instance;
};

CountingSemaphore<1>::StronglyTypedHandle CountingSemaphore<1>::create(const Attribute& attribute)
{
    Handle handle = nullptr;

    if (attribute.static_instance != nullptr && attribute.instance_size != 0U) {
        auto& static_instance = *static_cast<CountingSemaphore<1>::StaticInstance*>(attribute.static_instance);

        handle = xSemaphoreCreateBinaryStatic(&static_cast<StaticSemaphore_t&>(static_instance));
    }
    else {
        handle = xSemaphoreCreateBinary();
    }

#if (configQUEUE_REGISTRY_SIZE > 0)
    if (handle != nullptr) {
        vQueueAddToRegistry(static_cast<QueueHandle_t>(handle), attribute.name);
    }
#endif

    return {handle};
}

void CountingSemaphore<1>::destroy(CountingSemaphore<1>& obj)
{
    if (obj.handle_ != nullptr) {
        vSemaphoreDelete(obj.handle_);
        obj.handle_ = nullptr;
    }
}

inline void CountingSemaphore<1>::release(bool in_isr) noexcept
{
    if (in_isr) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        xSemaphoreGiveFromISR(handle_, &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else {
        xSemaphoreGive(handle_);
    }
}

inline ErrorCode CountingSemaphore<1>::acquire(bool in_isr, SysTick_t timeout_tick) noexcept
{
    if (in_isr) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        auto ret = xSemaphoreTakeFromISR(handle_, &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        return (ret == pdTRUE ? ErrorCode::Success : ErrorCode::OperationFail);
    }
    else {
        auto ret = xSemaphoreTake(static_cast<SemaphoreHandle_t>(handle_), timeout_tick);

        return (ret == pdTRUE ? ErrorCode::Success : ErrorCode::Timeout);
    }
}

CountingSemaphore<1>::~CountingSemaphore()
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif
