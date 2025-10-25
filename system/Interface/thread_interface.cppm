/**
 * @file thread_interface.cppm
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
#include "emdevif/fatal_handler.h"

export module emdevif.sys.thread:interface;

export import emdevif.errorHandler;
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

/**
 * 线程类
 */
class Thread
{
public:
    using ThreadEntry = void (*)(void*);  ///< 线程标准入口函数指针
    using Handle = void*;                 ///< 线程句柄

    template<std::size_t stack_depth>
    class StaticInstance;

    /**
     * 优先级
     */
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

    /**
     * 属性，作为 create 方法的参数类型。
     */
    struct Attribute {
        const char* name{};                   ///< 名称。必填参数。
        Priority priority{Priority::Normal};  ///< 初始的优先级（默认为常规优先级）

        void* static_instance{nullptr};       ///< 静态实例内存
        std::size_t stack_size{0U};           ///< 栈深度
    };

    /**
     * 优先级映射范围。
     *
     * 由于 Priority 只有 8 个等级，而实际的系统可能有更多的优先级，因此需要把 Priority
     * 中的等级映射到实际系统的优先级中。这个映射的方式是：保持靠左剧中。例如实际系统有 9
     * 个优先级，从 0 ~ 8 分别对应优先级最低到最高，那么 Priority 的 Low ~ Realtime
     * 将会映射到实际优先级的 1 ~ 6，Priority::Idle 与 Priority::Max 则分别对应 0 和 8。
     * @return std::pair 类型，值分别是 Priority::Low 与 Priority::Realtime
     *         映射到实际系统的优先级。
     */
    static consteval auto priorityMapRange() noexcept;

private:
    /**
     * 强类型句柄
     *
     * 用于 Thread::create 的返回值，同时也是 Thread 的其中一个构造函数的参数类型。
     * 将 Handle 包装一层结构体，用于区分类型，同时提醒用户不要直接将 void* 类型传入构造函数。
     */
    struct StronglyTypedHandle {
        Handle value;
    };

    /**
     * 将给定的 Priority 中的优先级映射到实际系统的优先级
     * @param priority 优先级
     * @return 这个优先级映射到实际系统的优先级的值
     */
    static constexpr auto priorityMapToSystem(Priority priority);

public:
    /**
     * 等待时间最大值
     */
    static consteval SysTick_t maxDelay() noexcept;

    /**
     * 获取当前系统时间戳的时间值
     * @param in_isr 是否在中断中调用的这个函数
     * @return 当前系统时间戳的时间值
     */
    static SysTick_t getTick(bool in_isr);

    /**
     * 延时
     * @param ticks 需要延时的 tick 数
     */
    static void delay(SysTick_t ticks);

    /**
     * 绝对延时
     *
     * 示例：@code
     * import emdevif.sys.thread;
     * using namespace emdevif;
     *
     * constexpr auto delay_ticks = Thread::msToTick(100);
     * Thread::delayUntil(Thread::getTick(false) + delay_ticks);  // 不能在中断里使用
     * @endcode
     * @param ticks 要等待到的绝对时间
     * @return 如果成功延时，返回 Success。如果预计等待到的绝对时间已经过去，返回 InvalidArgument。
     */
    static ErrorCode delayUntil(SysTick_t ticks);

    /**
     * 将毫秒时间转化成 tick 单位
     * @param ms 毫秒
     * @return 转化后的 tick 数
     */
    static SysTick_t msToTick(SysTick_t ms);

    /**
     * 创建线程
     *
     * 动态创建的使用示例：@code
     * import emdevif.sys.thread;
     * using namespace emdevif;
     *
     * void threadEntry(void* arguments)
     * {
     *     auto& arg_ = *static_cast<int*>(arguments);  // arg_ 是传入的参数 arg 的引用
     *     // 使用时请注意 arg 的生命周期，避免悬垂引用。
     *
     *     // do something...
     * }
     *
     * int arg;
     *
     * void init()
     * {
     *     Thread thread;  // 此时，这个线程未创建
     *
     *     thread = Thread::create({.name = "thread"}, threadEntry, &arg);
     *     // 第一个参数不填写 .static_instance 和 .stack_size 即为动态创建
     *
     *     // thread 离开作用域后会自动删除，因此您可以将它定义在全局区域内
     * }
     * @endcode
     *
     * 静态创建的使用示例：@code
     * import emdevif.sys.thread;
     * using namespace emdevif;
     *
     * constexpr auto thread_instance_stack_size = 128;
     * static Thread::StaticInstance<thread_instance_stack_size> thread_instance;
     * Thread thread;  // 静态创建的线程不允许删除，因此它必须在全局区定义或者加上 static
     *
     * void init()
     * {
     *     thread = Thread::create({.name = "thread",
     *                              .static_instance = &thread_instance,
     *                              .stack_size = thread_instance_stack_size
     *                             },
     *                             threadEntry,
     *                             &arg);
     * }
     * @endcode
     * @param attribute 属性。结构体成员详见 Attribute 的说明
     * @param entry 线程标准入口，即一个签名为 void(void*) 的函数
     * @param arguments 传入线程标准入口的参数
     * @return 创建的强类型句柄。
     */
    static StronglyTypedHandle create(const Attribute& attribute, ThreadEntry entry, void* arguments) noexcept;

private:
    /**
     * 线程任意入口的函数包装器
     * @tparam Func 线程的入口函数（函数返回值必须是 void，参数任意）
     * @tparam Args 这个入口函数的参数包
     */
    template<typename Func, typename... Args>
    class FuncWrapper final
    {
    public:
        explicit FuncWrapper(Func&& func, Args&&... args) noexcept
            : func_(std::forward<Func>(func)), args_(std::forward<Args>(args)...)
        {
        }

        /**
         * 实际传入创建线程方法的函数
         * @param arguments 包装后的函数参数
         */
        static void realFunc(void* arguments) noexcept
        {
            auto arg_pack = static_cast<FuncWrapper*>(arguments);

            std::apply(arg_pack->func_, arg_pack->args_);
        }

        Func func_;                 ///< 函数对象
        std::tuple<Args...> args_;  ///< 函数的参数包
    };

    /**
     * 多参数的线程函数的线程的强类型句柄
     */
    struct MulParamThreadFuncHandle {
        Handle handle;                    ///< 句柄
        void* func_wrapper_memory_block;  ///< 储存形参包的内存块的地址
    };

public:
    enum class MulParam {
        mulparam
    };
    /// 多参数线程入口创建标志
    static constexpr auto mulparam = MulParam::mulparam;

    /**
     * 创建任意参数的入口函数的线程
     *
     * 使用示例：@code
     * #include <functional>  // std::ref requires
     *
     * import emdevif.sys.thread;
     * using namespace emdevif;
     *
     * Thread thread;
     *
     * void init()
     * {
     *     int capture;
     *     int ref_capture;
     *     int ref_value;
     *
     *     thread = Thread::create(
     *         {.name = "thread"},
     *         Thread::mulparam,  // 这个参数用于表示这个函数是任意类型的参数
     *         [capture, &ref_capture](int a, float f, int& ref) {  // 函数对象、有捕获列表的 lambda 表达式也可以
     *             // do something...
     *         },
     *         1,
     *         123.456f,
     *         std::ref(ref_value)
     *     );
     * }
     * @endcode
     * @attention 会使用系统的堆内存，因此需要启动调度器后才能创建（不能静态创建）。
     * @tparam Func 线程的入口函数的类型
     * @tparam Args 这个入口函数的参数包
     * @param attribute 属性
     * @param Mulparam 必须传入 Thread::mulparam 以表示创建的线程的入口函数的参数类型、数量是任意的
     * @param entry 线程的入口函数（可以是函数对象。函数返回值必须是 void，参数类型任意）
     * @param args 入口函数的参数
     * @return 创建好的多参数的线程函数的线程的强类型句柄
     */
    template<typename Func, typename... Args>
    static auto create(const Attribute& attribute, MulParam, Func&& entry, Args&&... args) noexcept
        -> MulParamThreadFuncHandle
    {
        auto func_wrapper = heap::construct<FuncWrapper<Func, Args...>>(std::nothrow,
                                                                        std::forward<Func>(entry),
                                                                        std::forward<Args>(args)...);
        if (func_wrapper == nullptr) {
            return {.handle = nullptr, .func_wrapper_memory_block = nullptr};
        }

        return {.handle = create(attribute, func_wrapper->realFunc, func_wrapper).value,
                .func_wrapper_memory_block = static_cast<void*>(func_wrapper)};
    }

    /**
     * 删除线程
     * @attention 不能删除静态创建的线程
     * @param obj 待删除的线程实例
     * @return 成功删除返回 Success；传入未创建好的或者已删除的线程实例返回 InvalidArgument
     */
    static ErrorCode destroy(Thread& obj);

    /**
     * 退出当前线程
     *
     * 使用示例：@code
     * import emdevif.sys.thread;
     * using namespace emdevif;
     *
     * Thread thread;
     *
     * void threadEntry(Thread& th)
     * {
     *     // do something...
     *
     *     th.exit();  // 退出当前线程
     *     // 注意：如果需要退出线程，不能直接退出，必须调用 th.exit() 才行。
     *
     *     // 程序将不会运行到此处
     * }
     *
     * // 创建线程的示例参考 Thread::create 方法
     * @endcode
     * @attention[1] 这个函数一旦调用就不会返回。
     * @attention[2] 静态创建的线程不能退出。
     */
    EMDEVIF_NO_RETURN void exit();

    /**
     * 挂起线程
     * @param handle 待挂起的线程的句柄（可以通过 getHandle 方法获取）
     */
    static void suspend(Handle handle);

    /**
     * 恢复被挂起的线程
     * @param in_isr 是否在中断里调用
     * @param handle 待恢复的线程的句柄（可以通过 getHandle 方法获取）
     */
    static void resume(bool in_isr, Handle handle);

    /**
     * 启动调度器
     */
    static void startScheduler();

    /**
     * 关闭调度器
     */
    static void endScheduler();

    /**
     * 向实现提供一个提示，以重新调度当前线程的执行，允许其他线程运行。
     * @attention 此函数的具体行为取决于底层系统的实现，需要参考实际系统的文档。
     */
    static void yield();

    /**
     * 检查当前对象是否可连接的
     * @return 可连接的状态
     */
    bool joinable() noexcept;

    /**
     * 连接线程。即阻塞当前线程，直到实例线程执行完毕
     */
    void join();

    /**
     * 获得底层实现的句柄
     * @return 一个可选值，如果线程存在，返回底层实现的句柄，否则返回 std::nullopt
     */
    [[nodiscard]] Handle getHandle() const
    {
        return handle_;
    }

    Thread() : handle_(nullptr) {}

    Thread(const StronglyTypedHandle strongly_handle) : handle_(strongly_handle.value) {}  // NOLINT

    Thread& operator=(const StronglyTypedHandle strongly_handle)
    {
        if (handle_ != nullptr) {
            EMDEVIF_FATAL_HANDLER("Should not create thread on non-deleted thread!");
            return *this;
        }

        handle_ = strongly_handle.value;

        return *this;
    }

    Thread(const Attribute& attribute, const ThreadEntry entry, void* arguments)
        : Thread(create(attribute, entry, arguments))
    {
    }

    Thread(const MulParamThreadFuncHandle mul_param_thread_func_handle)  // NOLINT
        : handle_(mul_param_thread_func_handle.handle),
          func_wrapper_memory_block_(mul_param_thread_func_handle.func_wrapper_memory_block)
    {
    }

    Thread& operator=(const MulParamThreadFuncHandle mul_param_thread_func_handle)
    {
        if (handle_ != nullptr || func_wrapper_memory_block_ != nullptr) {
            EMDEVIF_FATAL_HANDLER("Should not create thread on non-deleted thread!");
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
