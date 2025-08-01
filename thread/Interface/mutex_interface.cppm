/**
 * @file mutex_interface.cppm
 * @module emdevif.mutex:interface
 * @author DuYicheng
 * @date 2025-08-01
 * @brief 互斥锁接口
 */

module;

#include <cstdint>

export module emdevif.mutex:interface;

import emdevif.error_handler;
import emdevif.thread;

export namespace emdevif {

class Mutex
{
public:
    using Handle = void*;

    struct Attribute {
        const char* name{};     ///< 名称
        void* cb_mem{nullptr};  ///< 控制块内存
        uint32_t cb_size{};     ///< 控制块大小
    };

    struct StronglyTypedHandle {
        Handle value;
    };

    static StronglyTypedHandle create(const Attribute& attribute);

    static void destroy(Handle handle);

    void destroy()
    {
        destroy(handle_);
        handle_ = nullptr;
    }

    // NOLINTNEXTLINE
    ErrorCode lock(uint32_t timeout_tick = Thread::MAX_DELAY()) const;

    [[nodiscard]] ErrorCode try_lock() const;

    void unlock() const;

    Mutex() : handle_(nullptr) {}

    explicit Mutex(StronglyTypedHandle strongly_handle) : handle_(strongly_handle.value) {}

    explicit Mutex(const Attribute& attribute) : Mutex(create(attribute)) {}

private:
    Handle handle_;
};

}  // namespace emdevif
