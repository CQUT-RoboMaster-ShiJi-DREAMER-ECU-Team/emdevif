/**
 * @file msg_queue_interface.cppm
 * @module emdevif.sys.message_queue:interface
 * @author 杜以成
 * @date 2025-06-18
 * @brief 消息队列接口
 */

module;

#include <cstdint>

#include "emdevif/fault_handler.hpp"

export module emdevif.sys.message_queue:interface;

export import emdevif.error_handler;

namespace emdevif {

template<typename Type = void*, std::size_t size = 0>
class MessageQueue
{
public:
    using Handle = void*;

    struct Attribute {
        const char* name{};     ///< 名称
        void* cb_mem{nullptr};  ///< 控制块的内存
        uint32_t cb_size{};     ///< 控制块的大小
        void* mq_mem{nullptr};  ///< 队列数据存储内存
        uint32_t mq_size{};     ///< 队列数据存储的内存大小
    };

private:
    struct StronglyTypedHandle {
        Handle value;
    };

public:
    static StronglyTypedHandle create(const Attribute& attribute);

    static void destroy(MessageQueue& obj);

    void destroy()
    {
        destroy(*this);
        handle_ = nullptr;
    }

    ErrorCode push(bool in_isr, const Type& data, std::size_t timeout = 0U);

    ErrorCode pop(bool in_isr, Type& data, std::size_t timeout = 0U);

    ErrorCode pop(bool in_isr);

    ErrorCode peek(bool in_isr, Type& data, std::size_t timeout = 0U);

    [[nodiscard]] std::size_t storeCount() const;

    [[nodiscard]] std::size_t remainCount() const;

    [[nodiscard]] Handle getHandle() const
    {
        return handle_;
    }

    MessageQueue() : handle_(nullptr) {}

    explicit MessageQueue(const StronglyTypedHandle strongly_handle) : handle_(strongly_handle.value) {}

    MessageQueue& operator=(const MessageQueue&) = delete;
    MessageQueue(const MessageQueue&) = delete;

    MessageQueue& operator=(const StronglyTypedHandle strongly_handle)
    {
        if (handle_ != nullptr) {
            EMDEVIF_FAULT_HANDLER("Should not create message queue on non-deleted message queue!");
            return *this;
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    explicit MessageQueue(const Attribute& attribute) : MessageQueue(create(attribute)) {}

    MessageQueue(MessageQueue&& other) noexcept : handle_(other.handle_)
    {
        other.handle_ = nullptr;
    }

    MessageQueue& operator=(MessageQueue&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        this->handle_ = other.handle_;
        other.handle_ = nullptr;

        return *this;
    }

    ~MessageQueue();

private:
    Handle handle_;
};

}  // namespace emdevif
