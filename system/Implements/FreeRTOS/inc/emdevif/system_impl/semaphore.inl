/**
 * @file semaphore.inl
 * @brief
 */

#pragma once
#ifndef EMDEVIF_FREERTOS_SYSTEM_IMPL_SEMAPHORE_INL
#define EMDEVIF_FREERTOS_SYSTEM_IMPL_SEMAPHORE_INL

#include "emdevif/core/detail/config.hpp"

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#else
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#include "emdevif/core/error_handler.hpp"

#include "emdevif/core/fatal_handler.h"

#include <cstddef>
#endif

// for template<std::ptrdiff_t least_max_value> CountingSemaphore
EMDEVIF_MODULE_EXPORT
namespace emdevif {

template<std::ptrdiff_t least_max_value>
class CountingSemaphore<least_max_value>::StaticInstance
{
public:
    StaticInstance() noexcept : instance() {}

    friend class CountingSemaphore<least_max_value>;

private:
    explicit operator StaticSemaphore_t&() noexcept
    {
        return instance;
    }

private:
    StaticSemaphore_t instance;  ///< 静态创建所需的内存块
};

template<std::ptrdiff_t least_max_value>
auto CountingSemaphore<least_max_value>::create(const Attribute& attribute) noexcept
    -> CountingSemaphore<least_max_value>::StronglyTypedHandle
{
    Handle handle = nullptr;

    if (attribute.static_instance != nullptr) {
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
void CountingSemaphore<least_max_value>::destroy(CountingSemaphore& obj) noexcept
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
CountingSemaphore<least_max_value>::~CountingSemaphore() noexcept
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif

// for template<> CountingSemaphore<1>
EMDEVIF_MODULE_EXPORT
namespace emdevif {

template<>
class CountingSemaphore<1>
{
public:
    using Handle = void*;

    class StaticInstance;

    struct Attribute {
        const char* name{};                        ///< 名称

        StaticInstance* static_instance{nullptr};  ///< 静态实例内存
    };

private:
    struct StronglyTypedHandle {
        Handle value;
    };

public:
    static StronglyTypedHandle create(const Attribute& attribute) noexcept;

    static void destroy(CountingSemaphore& obj) noexcept;

    void destroy() noexcept
    {
        destroy(*this);
        handle_ = nullptr;
    }

    void release(bool in_isr) noexcept;

    ErrorCode acquire(bool in_isr, SysTick_t timeout_tick = Thread::maxDelay()) noexcept;

    ErrorCode try_acquire(const bool in_isr) noexcept
    {
        return acquire(in_isr, 0U);
    }

    [[nodiscard]] Handle getHandle() const noexcept
    {
        return handle_;
    }

    CountingSemaphore() noexcept : handle_(nullptr) {}

    explicit CountingSemaphore(const StronglyTypedHandle strongly_handle) noexcept : handle_(strongly_handle.value) {}

    CountingSemaphore& operator=(const CountingSemaphore&) = delete;
    CountingSemaphore(const CountingSemaphore&) = delete;

    CountingSemaphore& operator=(const StronglyTypedHandle strongly_handle) noexcept
    {
        if (handle_ != nullptr) {
            EMDEVIF_FATAL_HANDLER("Should not create binary semaphore on non-deleted binary semaphore!");
            return *this;
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    explicit CountingSemaphore(const Attribute& attribute) noexcept : CountingSemaphore(create(attribute)) {}

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

    ~CountingSemaphore() noexcept;

private:
    Handle handle_;
};

class CountingSemaphore<1>::StaticInstance
{
public:
    StaticInstance() noexcept : instance() {}

    friend class CountingSemaphore<1>;

    [[nodiscard]] constexpr std::size_t getInstanceSize() const noexcept
    {
        return sizeof instance;
    }

private:
    explicit operator StaticSemaphore_t&() noexcept
    {
        return instance;
    }

private:
    StaticSemaphore_t instance;
};

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

}  // namespace emdevif

#endif  // !EMDEVIF_FREERTOS_SYSTEM_IMPL_SEMAPHORE_INL
