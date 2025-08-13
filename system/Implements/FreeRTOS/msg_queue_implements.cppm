/**
 * @file msg_queue_implements.cppm
 * @module emdevif.sys.message_queue:implements
 * @author 杜以成
 * @date 2025-06-18
 * @brief 消息队列的实现
 */

module;

#include <cstddef>

#include "FreeRTOS.h"
#include "queue.h"

export module emdevif.sys.message_queue:implements;
import :interface;

export namespace emdevif {

template<typename Type, std::size_t size>
typename MessageQueue<Type, size>::StronglyTypedHandle MessageQueue<Type, size>::create(const Attribute& attribute)
{
    Handle new_handle = nullptr;

    if (attribute.cb_mem != nullptr && attribute.cb_size != 0 && attribute.mq_mem != nullptr &&
        attribute.mq_size != 0) {
        new_handle =
            xQueueCreateStatic(size, sizeof(Type), attribute.mq_mem, static_cast<StaticQueue_t>(attribute.cb_mem));
    }
    else {
        new_handle = xQueueCreate(size, sizeof(Type));
    }

#if (configQUEUE_REGISTRY_SIZE > 0)
    if (new_handle != nullptr) {
        vQueueAddToRegistry(new_handle, attribute.name);
    }
#endif

    return {new_handle};
}

template<typename Type, std::size_t size>
void MessageQueue<Type, size>::destroy(MessageQueue& obj)
{
    if (obj.handle_ != nullptr) {
        vQueueDelete(obj.handle_);

#if (configQUEUE_REGISTRY_SIZE > 0)
        vQueueUnregisterQueue(obj.handle_);
#endif

        obj.handle_ = nullptr;
    }
}

template<typename Type, std::size_t size>
ErrorCode MessageQueue<Type, size>::push(const bool in_isr, const Type& data, std::size_t timeout)
{
    if (in_isr) {
        BaseType_t xHigherPriorityTaskWokenByPost = pdFALSE;
        const auto ret = xQueueSendFromISR(handle_, &data, &xHigherPriorityTaskWokenByPost);

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
        const auto ret = xQueueSend(handle_, &data, timeout);
        if (ret == pdTRUE) {
            return ErrorCode::Success;
        }
        else {
            return ErrorCode::Full;
        }
    }
}

template<typename Type, std::size_t size>
ErrorCode MessageQueue<Type, size>::pop(const bool in_isr, Type& data, std::size_t timeout)
{
    if (in_isr) {
        BaseType_t xHigherPriorityTaskWokenByPost = pdFALSE;
        const auto ret = xQueueReceiveFromISR(handle_, &data, &xHigherPriorityTaskWokenByPost);

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
        const auto ret = xQueueReceive(handle_, &data, timeout);
        if (ret == pdTRUE) {
            return ErrorCode::Success;
        }
        else {
            return ErrorCode::Empty;
        }
    }
}

template<typename Type, std::size_t size>
ErrorCode MessageQueue<Type, size>::pop(const bool in_isr)
{
    Type data;
    return pop(in_isr, &data, 0U);
}

template<typename Type, std::size_t size>
ErrorCode MessageQueue<Type, size>::peek(const bool in_isr, Type& data, std::size_t timeout)
{
    if (in_isr) {
        const auto ret = xQueuePeekFromISR(handle_, &data);

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
        const auto ret = xQueuePeek(handle_, &data, timeout);
        if (ret == pdTRUE) {
            return ErrorCode::Success;
        }
        else {
            return ErrorCode::Empty;
        }
    }
}

template<typename Type, std::size_t size>
std::size_t MessageQueue<Type, size>::storeCount() const
{
    return uxQueueMessagesWaiting(static_cast<QueueHandle_t>(handle_));
}

template<typename Type, std::size_t size>
std::size_t MessageQueue<Type, size>::remainCount() const
{
    return uxQueueSpacesAvailable(static_cast<QueueHandle_t>(handle_));
}

template<typename Type, std::size_t size>
MessageQueue<Type, size>::~MessageQueue()
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif
