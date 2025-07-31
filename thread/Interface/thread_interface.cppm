/**
 * @file thread_interface.cppm
 * @module emdevif.thread:interface
 * @author 杜以成
 * @date 2025-06-18
 * @brief 线程相关常数接口
 */

module;

#include "emdevif/attributes_and_useful_macros.h"

#include <cstdint>

export module emdevif.thread:interface;

export import emdevif.error_handler;

export namespace emdevif {

class Thread
{
public:
    using ThreadEntry = void (*)(void*);
    using Handle = void*;

    struct Attribute {
        const char* name{};        ///< 名称
        int32_t priority{};        ///< 初始的优先级
        void* cb_mem{nullptr};     ///< 控制块的内存
        uint32_t cb_size{};        ///< 控制块的大小
        void* stack_mem{nullptr};  ///< 存储栈的内存
        uint32_t stack_size{};     ///< 存储栈的内存大小
    };

    /**
     * 等待时间最大值
     */
    static consteval auto MAX_DELAY() noexcept;

    static auto getTick(bool in_isr);

    static void delay(uint32_t ticks);

    static void delayMs(uint32_t ms);

    static ErrorCode delayUntil(uint32_t ticks);

    static Handle create(ThreadEntry entry, void* arguments, const Attribute& attribute);

    static void destroy(Handle handle);

    EMDEVIF_NO_RETURN static void exit();

    [[nodiscard]] Handle getHandle() const
    {
        return handle_;
    }

    Thread() : handle_(nullptr) {}

    explicit Thread(Handle handle) : handle_(handle) {}

    Thread(const ThreadEntry entry, void* arguments, const Attribute& attribute)
        : handle_(create(entry, arguments, attribute))
    {
    }

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    Thread(Thread&& other) noexcept : handle_(other.handle_)
    {
        other.handle_ = nullptr;
    }

    Thread& operator=(Thread&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        this->handle_ = other.handle_;
        other.handle_ = nullptr;

        return *this;
    }

    ~Thread()
    {
        handle_ = nullptr;
    }

private:
    Handle handle_;
};

}  // namespace emdevif
