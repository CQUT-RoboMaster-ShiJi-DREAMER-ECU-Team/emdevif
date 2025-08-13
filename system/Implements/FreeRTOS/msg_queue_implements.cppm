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

#include "emdevif/byte_type.h"

#include "emdevif/fault_handler.hpp"

export module emdevif.sys.message_queue:implements;
import :interface;

namespace emdevif {

export template<typename Type, std::size_t length>
class MessageQueueView : public MessageQueueInterface<MessageQueueView<Type, length>>
{
public:
    using DataType = Type;

    MessageQueueView() = delete;

    explicit MessageQueueView(QueueHandle_t handle) : handle_(handle)
    {
        if (handle == nullptr) {
            EMDEVIF_FAULT_HANDLER("Failed to create message queue!");
        }
    }

    [[nodiscard]] QueueHandle_t getHandle() const
    {
        return this->handle_;
    }

protected:
    ErrorCode push_impl(const bool in_isr, const Type& data, const std::size_t timeout)
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

    ErrorCode pop_impl(const bool in_isr, Type& data, const std::size_t timeout)
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

    ErrorCode pop_impl(const bool in_isr)
    {
        Type data;
        return pop_impl(in_isr, &data, 0U);
    }

    ErrorCode peek_impl(bool in_isr, Type& data, std::size_t timeout)
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

    [[nodiscard]] std::size_t size_impl() const
    {
        return uxQueueMessagesWaiting(handle_);
    }

    [[nodiscard]] std::size_t remainSize_impl() const
    {
        return uxQueueSpacesAvailable(handle_);
    }

protected:
    QueueHandle_t handle_;
};

export template<typename Type, std::size_t length>
class MessageQueue : MessageQueueView<Type, length>
{
public:
    MessageQueue()
        : MessageQueueView<Type, length>(xQueueCreateStatic(length,
                                                            sizeof(typename MessageQueueView<Type, length>::DataType),
                                                            storge_memory,
                                                            &queue_buffer))
    {
    }

private:
    StaticQueue_t queue_buffer{};
    ubyte_t storge_memory[length * sizeof(Type)]{};
};

}  // namespace emdevif
