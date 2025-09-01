/**
 * @file thread_interface.cppm
 * @module emdevif.sys.thread:interface
 * @author 杜以成
 * @date 2025-06-18
 * @brief 线程接口
 */

module;

#include <cstdint>

#include <optional>
#include <tuple>
#include <utility>

#include "emdevif/attributes_and_useful_macros.h"
#include "emdevif/fault_handler.hpp"

export module emdevif.sys.thread:interface;

export import emdevif.error_handler;
import emdevif.sys.heap;

export namespace emdevif {

#ifndef EMDEVIF_SYS_TICK_TYPE_UINT_BITS
#define EMDEVIF_SYS_TICK_TYPE_UINT_BITS 64
#endif

#if (EMDEVIF_SYS_TICK_TYPE_UINT_BITS == 16)
using SysTick_t = uint16_t;
#elif (EMDEVIF_SYS_TICK_TYPE_UINT_BITS == 32)
using SysTick_t = uint32_t;
#elif (EMDEVIF_SYS_TICK_TYPE_UINT_BITS == 64)
using SysTick_t = uint64_t;
#else
#error "The value of macro `EMDEVIF_SYS_TICK_TYPE_UINT_BITS' can only choose 16, 32 or 64."
#endif

class Thread
{
public:
    using ThreadEntry = void (*)(void*);
    using Handle = void*;

    template<std::size_t stack_depth>
    class StaticInstance;

    enum class Priority : int_fast8_t {
        Idle = -1,    ///< 空闲任务优先级（确保与空闲优先级一致）

        Low = 0,      ///< 低优先级
        BelowNormal,  ///< 低于常规优先级
        Normal,       ///< 常规优先级（默认值）
        AboveNormal,  ///< 高于常规优先级
        High,         ///< 高优先级
        Realtime,     ///< 实时优先级

        Max = -2      ///< 最高优先级（确保与最大优先级一致）
    };

    struct Attribute {
        const char* name{};                   ///< 名称
        Priority priority{Priority::Normal};  ///< 初始的优先级（默认为常规优先级）

        void* static_instance{nullptr};       ///< 静态实例内存
        std::size_t stack_size{0U};           ///< 栈深度
    };

    static consteval auto priorityMapRange() noexcept;

private:
    struct StronglyTypedHandle {
        Handle value;
    };

    static auto priorityMapToSystem(Priority priority);

public:
    /**
     * 等待时间最大值
     */
    static consteval auto MAX_DELAY() noexcept;

    static auto getTick(bool in_isr);

    static void delay(SysTick_t ticks);

    static ErrorCode delayUntil(SysTick_t ticks);

    static auto msToTick(SysTick_t ms);

    static StronglyTypedHandle create(const Attribute& attribute, ThreadEntry entry, void* arguments) noexcept;

private:
    template<typename Func, typename... Args>
    class FuncWrapper final
    {
    public:
        explicit FuncWrapper(const Func& func, Args&&... args) noexcept
            : func_(func), args_(std::forward<Args>(args)...)
        {
        }

        static void realFunc(void* arguments) noexcept
        {
            auto arg_pack = static_cast<FuncWrapper*>(arguments);

            constexpr auto element_count = std::tuple_size_v<decltype(arg_pack->args_)>;
            static_assert(element_count <= 8, "The parameter of function should be less than or equal to 8.");
            if constexpr (element_count == 1) {
                arg_pack->func_(std::get<0>(arg_pack->args_));
            }
            else if constexpr (element_count == 2) {
                arg_pack->func_(std::get<0>(arg_pack->args_), std::get<1>(arg_pack->args_));
            }
            else if constexpr (element_count == 3) {
                arg_pack->func_(std::get<0>(arg_pack->args_),
                                std::get<1>(arg_pack->args_),
                                std::get<2>(arg_pack->args_));
            }
            else if constexpr (element_count == 4) {
                arg_pack->func_(std::get<0>(arg_pack->args_),
                                std::get<1>(arg_pack->args_),
                                std::get<2>(arg_pack->args_),
                                std::get<3>(arg_pack->args_));
            }
            else if constexpr (element_count == 5) {
                arg_pack->func_(std::get<0>(arg_pack->args_),
                                std::get<1>(arg_pack->args_),
                                std::get<2>(arg_pack->args_),
                                std::get<3>(arg_pack->args_),
                                std::get<4>(arg_pack->args_));
            }
            else if constexpr (element_count == 6) {
                arg_pack->func_(std::get<0>(arg_pack->args_),
                                std::get<1>(arg_pack->args_),
                                std::get<2>(arg_pack->args_),
                                std::get<3>(arg_pack->args_),
                                std::get<4>(arg_pack->args_),
                                std::get<5>(arg_pack->args_));
            }
            else if constexpr (element_count == 7) {
                arg_pack->func_(std::get<0>(arg_pack->args_),
                                std::get<1>(arg_pack->args_),
                                std::get<2>(arg_pack->args_),
                                std::get<3>(arg_pack->args_),
                                std::get<4>(arg_pack->args_),
                                std::get<5>(arg_pack->args_),
                                std::get<6>(arg_pack->args_));
            }
            else if constexpr (element_count == 8) {
                arg_pack->func_(std::get<0>(arg_pack->args_),
                                std::get<1>(arg_pack->args_),
                                std::get<2>(arg_pack->args_),
                                std::get<3>(arg_pack->args_),
                                std::get<4>(arg_pack->args_),
                                std::get<5>(arg_pack->args_),
                                std::get<6>(arg_pack->args_),
                                std::get<7>(arg_pack->args_));
            }
        }

        Func func_;
        std::tuple<Args...> args_;
    };

    struct MulParamThreadFuncHandle {
        Handle handle;
        void* func_wrapper_memory_block;
    };

public:
    enum class MulParam {
        mulparam
    };
    static constexpr auto mulparam = MulParam::mulparam;

    template<typename Func, typename... Args>
    static auto create(const Attribute& attribute, MulParam, const Func& entry, Args&&... args) noexcept
        -> MulParamThreadFuncHandle
    {
        auto func_wrapper =
            heap::construct<FuncWrapper<Func, Args...>>(std::nothrow, entry, std::forward<Args>(args)...);
        if (func_wrapper == nullptr) {
            return {.handle = nullptr, .func_wrapper_memory_block = nullptr};
        }

        return {.handle = create(attribute, func_wrapper->realFunc, func_wrapper).value,
                .func_wrapper_memory_block = static_cast<void*>(func_wrapper)};
    }

    static ErrorCode destroy(Thread& obj);

    EMDEVIF_NO_RETURN void exit();

    static void suspend(Handle handle);

    static void resume(bool in_isr, Handle handle);

    static void startScheduler();

    static void endScheduler();

    static void yield();

    void join();

    [[nodiscard]] std::optional<Handle> getHandle() const
    {
        if (handle_ == nullptr) {
            return std::nullopt;
        }

        return handle_;
    }

    Thread() : handle_(nullptr) {}

    explicit Thread(const StronglyTypedHandle strongly_handle) : handle_(strongly_handle.value) {}

    Thread& operator=(const StronglyTypedHandle strongly_handle)
    {
        if (handle_ != nullptr) {
            EMDEVIF_FAULT_HANDLER("Should not create thread on non-deleted thread!");
            return *this;
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    Thread(const Attribute& attribute, const ThreadEntry entry, void* arguments)
        : Thread(create(attribute, entry, arguments))
    {
    }

    explicit Thread(const MulParamThreadFuncHandle mul_param_thread_func_handle)
        : handle_(mul_param_thread_func_handle.handle),
          func_wrapper_memory_block_(mul_param_thread_func_handle.func_wrapper_memory_block)
    {
    }

    Thread& operator=(const MulParamThreadFuncHandle mul_param_thread_func_handle)
    {
        if (handle_ != nullptr || func_wrapper_memory_block_ != nullptr) {
            EMDEVIF_FAULT_HANDLER("Should not create thread on non-deleted thread!");
            return *this;
        }

        handle_ = mul_param_thread_func_handle.handle;
        func_wrapper_memory_block_ = mul_param_thread_func_handle.func_wrapper_memory_block;

        return *this;
    }

    template<typename Func, typename... Args>
    Thread(const Attribute& attribute, MulParam, const Func& entry, Args&&... args)
        : Thread(create(attribute, mulparam, entry, std::forward<Args>(args)...))
    {
    }

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    Thread(Thread&& other) noexcept
        : handle_(other.handle_), func_wrapper_memory_block_(other.func_wrapper_memory_block_)
    {
        other.handle_ = nullptr;
        other.func_wrapper_memory_block_ = nullptr;
    }

    Thread& operator=(Thread&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        this->handle_ = other.handle_;
        this->func_wrapper_memory_block_ = other.func_wrapper_memory_block_;

        other.handle_ = nullptr;
        other.func_wrapper_memory_block_ = nullptr;

        return *this;
    }

    ~Thread();

private:
    Handle handle_;
    void* func_wrapper_memory_block_{nullptr};
};

}  // namespace emdevif
