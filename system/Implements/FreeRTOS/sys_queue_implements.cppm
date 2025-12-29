/**
 * @file sys_queue_implements.cppm
 * @brief 系统队列的实现
 */

module;

#include <cstddef>

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#else
#include "FreeRTOS.h"
#include "queue.h"
#endif

#include "emdevif/fatal_handler.h"

export module emdevif.sys.sysQueue:implements;
import :interface;

import emdevif.errorHandler;

export namespace emdevif {

template<typename Type, std::size_t item_size>
class SysQueue<Type, item_size>::StaticInstance
{
public:
    StaticInstance() noexcept : instance(), queue_buffer() {}

    explicit operator StaticQueue_t&() noexcept
    {
        return instance;
    }

    explicit operator void*() noexcept
    {
        return queue_buffer;
    }

private:
    StaticQueue_t instance;        ///< 静态队列实例
    Type queue_buffer[item_size];  ///< 队列数据存储缓冲区
};

template<typename Type, std::size_t item_size>
auto SysQueue<Type, item_size>::create(const Attribute& attribute) -> SysQueue<Type, item_size>::StronglyTypedHandle
{
    Handle new_handle = nullptr;

    if (attribute.static_instance != nullptr) {
        auto& static_instance = *static_cast<SysQueue<Type, item_size>::StaticInstance*>(attribute.static_instance);

        new_handle = xQueueCreateStatic(item_size,
                                        sizeof(Type),
                                        static_cast<uint8_t*>(static_cast<void*>(static_instance)),
                                        &static_cast<StaticQueue_t&>(static_instance));
    }
    else {
        new_handle = xQueueCreate(item_size, sizeof(Type));
    }

#if (configQUEUE_REGISTRY_SIZE > 0)
    if (new_handle != nullptr) {
        vQueueAddToRegistry(static_cast<QueueHandle_t>(new_handle), attribute.name);
    }
#endif

    return {new_handle};
}

template<typename Type, std::size_t item_size>
void SysQueue<Type, item_size>::destroy(SysQueue& obj)
{
    if (obj.handle_ != nullptr) {
        vQueueDelete(static_cast<QueueHandle_t>(obj.handle_));

#if (configQUEUE_REGISTRY_SIZE > 0)
        vQueueUnregisterQueue(static_cast<QueueHandle_t>(obj.handle_));
#endif

        obj.handle_ = nullptr;
    }
}

template<typename Type, std::size_t item_size>
ErrorCode SysQueue<Type, item_size>::pushImpl(const bool in_isr, const Type& data, const SysTick_t timeout_tick)
{
    if (in_isr) {
        EMDEVIF_ASSERT(timeout_tick == 0U, "Timeout value should equals to 0 in ISR.");

        BaseType_t xHigherPriorityTaskWokenByPost = pdFALSE;
        const auto ret = xQueueSendFromISR(static_cast<QueueHandle_t>(handle_), &data, &xHigherPriorityTaskWokenByPost);

        ErrorCode final_ret;

        if (ret == pdTRUE) {
            final_ret = ErrorCode::Success;
        }
        else {
            final_ret = ErrorCode::Full;
        }

        if (xHigherPriorityTaskWokenByPost) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWokenByPost);
        }

        return final_ret;
    }
    else {
        const auto ret = xQueueSend(static_cast<QueueHandle_t>(handle_), &data, timeout_tick);
        if (ret == pdTRUE) {
            return ErrorCode::Success;
        }
        else {
            return ErrorCode::Full;
        }
    }
}

template<typename Type, std::size_t item_size>
ErrorCode SysQueue<Type, item_size>::forcePushImpl(const bool in_isr, const Type& data)
{
    if (in_isr) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        auto ret = xQueueOverwriteFromISR(static_cast<QueueHandle_t>(handle_), &data, &xHigherPriorityTaskWoken);
        EMDEVIF_ASSERT(ret == pdPASS, "Failed to overwrite queue data in ISR context");

        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }

        return ErrorCode::Success;
    }
    else {
        auto ret = xQueueOverwrite(static_cast<QueueHandle_t>(handle_), &data);
        EMDEVIF_ASSERT(ret == pdPASS, "Failed to overwrite queue data in non-ISR context");

        return ErrorCode::Success;
    }
}

template<typename Type, std::size_t item_size>
ErrorCode SysQueue<Type, item_size>::popImpl(const bool in_isr, Type& data, const SysTick_t timeout_tick)
{
    if (in_isr) {
        EMDEVIF_ASSERT(timeout_tick == 0U, "Timeout value should equals to 0 in ISR.");

        BaseType_t xHigherPriorityTaskWokenByPost = pdFALSE;
        const auto ret =
            xQueueReceiveFromISR(static_cast<QueueHandle_t>(handle_), &data, &xHigherPriorityTaskWokenByPost);

        ErrorCode final_ret;

        if (ret == pdTRUE) {
            final_ret = ErrorCode::Success;
        }
        else {
            final_ret = ErrorCode::Empty;
        }

        if (xHigherPriorityTaskWokenByPost) {
            portYIELD_FROM_ISR(xHigherPriorityTaskWokenByPost);
        }

        return final_ret;
    }
    else {
        const auto ret = xQueueReceive(static_cast<QueueHandle_t>(handle_), &data, timeout_tick);
        if (ret == pdTRUE) {
            return ErrorCode::Success;
        }
        else {
            return ErrorCode::Empty;
        }
    }
}

template<typename Type, std::size_t item_size>
ErrorCode SysQueue<Type, item_size>::popImpl(const bool in_isr)
{
    Type data;
    return popImpl(in_isr, data, 0U);
}

template<typename Type, std::size_t item_size>
ErrorCode SysQueue<Type, item_size>::peekImpl(const bool in_isr, Type& data, const SysTick_t timeout_tick)
{
    if (in_isr) {
        EMDEVIF_ASSERT(timeout_tick == 0U, "Timeout value should equals to 0 in ISR.");

        const auto ret = xQueuePeekFromISR(static_cast<QueueHandle_t>(handle_), &data);

        ErrorCode final_ret;

        if (ret == pdTRUE) {
            final_ret = ErrorCode::Success;
        }
        else {
            final_ret = ErrorCode::Empty;
        }

        return final_ret;
    }
    else {
        const auto ret = xQueuePeek(static_cast<QueueHandle_t>(handle_), &data, timeout_tick);
        if (ret == pdTRUE) {
            return ErrorCode::Success;
        }
        else {
            return ErrorCode::Empty;
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
template<typename Type, std::size_t item_size>
void SysQueue<Type, item_size>::clearImpl()
{
    (void)xQueueReset(static_cast<QueueHandle_t>(handle_));
}

template<typename Type, std::size_t item_size>
std::size_t SysQueue<Type, item_size>::storeCountImpl() const
{
    return uxQueueMessagesWaiting(static_cast<QueueHandle_t>(handle_));
}

template<typename Type, std::size_t item_size>
std::size_t SysQueue<Type, item_size>::remainCountImpl() const
{
    return uxQueueSpacesAvailable(static_cast<QueueHandle_t>(handle_));
}

template<typename Type, std::size_t item_size>
SysQueue<Type, item_size>::~SysQueue()
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif
