/**
 * @file sys_queue_interface.cppm
 * @author 杜以成
 * @date 2025-06-18
 * @brief 系统队列接口
 */

module;

#include <cstdint>

#include "emdevif/fatal_handler.h"

export module emdevif.sys.sysQueue:interface;

export import emdevif.errorHandler;
export import emdevif.container.messageQueue;

export namespace emdevif {

/**
 * 系统队列
 * @tparam Type 队列每个元素的类型
 * @tparam item_size 队列的大小
 */
template<typename Type, std::size_t item_size>
class SysQueue : public MessageQueueInterface<SysQueue, Type, item_size>
{
public:
    using Handle = void*;  ///< 句柄

    class StaticInstance;

    /// 属性
    struct Attribute {
        const char* name{};                        ///< 名称

        StaticInstance* static_instance{nullptr};  ///< 静态实例内存
    };

private:
    struct StronglyTypedHandle {
        Handle value;
    };

public:  // todo 等消息队列接口做好了之后再来补注释
    static auto create(const Attribute& attribute) -> StronglyTypedHandle;

    static void destroy(SysQueue& obj);

    void destroy()
    {
        destroy(*this);
        handle_ = nullptr;
    }

    ErrorCode pushImpl(bool in_isr, const Type& data, std::size_t timeout = 0U);

    ErrorCode popImpl(bool in_isr, Type& data, std::size_t timeout = 0U);

    ErrorCode popImpl(bool in_isr);

    ErrorCode peekImpl(bool in_isr, Type& data, std::size_t timeout = 0U);

    [[nodiscard]] std::size_t storeCountImpl() const;

    [[nodiscard]] std::size_t remainCountImpl() const;

    [[nodiscard]] Handle getHandleImpl() const
    {
        return handle_;
    }

    SysQueue() : handle_(nullptr) {}

    explicit SysQueue(const StronglyTypedHandle strongly_handle) : handle_(strongly_handle.value) {}

    SysQueue& operator=(const SysQueue&) = delete;
    SysQueue(const SysQueue&) = delete;

    SysQueue& operator=(const StronglyTypedHandle strongly_handle)
    {
        if (handle_ != nullptr) {
            EMDEVIF_FATAL_HANDLER("Should not create message queue on non-deleted message queue!");
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    explicit SysQueue(const Attribute& attribute) : SysQueue(create(attribute)) {}

    SysQueue(SysQueue&& other) noexcept : handle_(other.handle_)
    {
        other.handle_ = nullptr;
    }

    SysQueue& operator=(SysQueue&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        this->handle_ = other.handle_;
        other.handle_ = nullptr;

        return *this;
    }

    ~SysQueue();

private:
    Handle handle_;
};

}  // namespace emdevif
