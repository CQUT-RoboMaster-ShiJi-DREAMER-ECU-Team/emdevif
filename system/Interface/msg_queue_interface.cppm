/**
 * @file msg_queue_interface.cppm
 * @module emdevif.sys.message_queue:interface
 * @author 杜以成
 * @date 2025-06-18
 * @brief 消息队列接口
 */

module;

#include <cstddef>

export module emdevif.sys.message_queue:interface;

export import emdevif.error_handler;

namespace emdevif {

template<class Impl>
class MessageQueueInterface
{
public:
    using Type = typename Impl::DataType;

    ErrorCode push(bool in_isr, const Type& data, std::size_t timeout = 0U)
    {
        return static_cast<Impl*>(this)->push_impl(in_isr, data, timeout);
    }

    ErrorCode pop(bool in_isr, Type& data, std::size_t timeout = 0U)
    {
        return static_cast<Impl*>(this)->pop_impl(in_isr, data, timeout);
    }

    ErrorCode pop(bool in_isr)
    {
        return static_cast<Impl*>(this)->pop_impl(in_isr);
    }

    ErrorCode peek(bool in_isr, Type& data, std::size_t timeout = 0U)
    {
        return static_cast<Impl*>(this)->peek_impl(in_isr, data, timeout);
    }

    [[nodiscard]] std::size_t size() const
    {
        return static_cast<const Impl*>(this)->size_impl();
    }

    [[nodiscard]] std::size_t remainSize() const
    {
        return static_cast<const Impl*>(this)->remainSize_impl();
    }

protected:
    MessageQueueInterface() = default;
    ~MessageQueueInterface() = default;
};

}  // namespace emdevif
