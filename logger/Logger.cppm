/**
 * @file Logger.cppm
 * @author DuYicheng
 * @date 2025-09-17
 * @brief 日志
 */

module;

#include <cstdint>
#include <cstdio>
#include <cstdarg>

#include <array>

#include "emdevif/attributes_and_useful_macros.h"
#include "emdevif/fatal_handler.h"
#include "emdevif/line_separator.h"

#ifndef EMDEVIF_LOGGER_BUFFER_SIZE
#define EMDEVIF_LOGGER_BUFFER_SIZE 256U
#endif

#ifndef EMDEVIF_LOGGER_BUFFER_COUNT
#define EMDEVIF_LOGGER_BUFFER_COUNT 4
#endif

#define EMDEVIF_LOGGER_MODE_SYNC  0
#define EMDEVIF_LOGGER_MODE_ASYNC 1

#ifndef EMDEVIF_LOGGER_MODE
#define EMDEVIF_LOGGER_MODE EMDEVIF_LOGGER_MODE_ASYNC
#endif

#ifndef EMDEVIF_LOGGER_SYNC_USE_LOCK
#define EMDEVIF_LOGGER_SYNC_USE_LOCK true
#endif

#ifndef EMDEVIF_LOGGER_ASYNC_THREAD_STACK_SIZE
#define EMDEVIF_LOGGER_ASYNC_THREAD_STACK_SIZE 128
#endif

export module emdevif.logger;

import emdevif.userDeclares;
import emdevif.errorHandler;

#if (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_ASYNC)
import emdevif.container.ringBuffer;
#endif

#if (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_ASYNC || \
     (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC && EMDEVIF_LOGGER_SYNC_USE_LOCK))
import emdevif.sys.thread;
import emdevif.sys.mutex;
import emdevif.sys.semaphore;
#define EMDEVIF_LOGGER_USE_MUTEX true
#else
#define EMDEVIF_LOGGER_USE_MUTEX false
#endif

namespace emdevif {

/**
 * 日志模式
 */
export enum class LoggerMode : uint_fast8_t {
    Sync = EMDEVIF_LOGGER_MODE_SYNC,   ///< 同步模式
    Async = EMDEVIF_LOGGER_MODE_ASYNC  ///< 异步模式
};

/// 当前配置下的日志模式
export constexpr auto logger_mode = static_cast<LoggerMode>(EMDEVIF_LOGGER_MODE);

export constexpr std::size_t logger_buffer_size = EMDEVIF_LOGGER_BUFFER_SIZE;    ///< 一个日志缓冲区的大小
export constexpr std::size_t logger_buffer_count = EMDEVIF_LOGGER_BUFFER_COUNT;  ///< 日志缓冲区数量

/**
 * 日志等级
 */
export enum class LogLevel : uint_fast8_t {
    Debug = 0,  ///< 调试等级
    Info,       ///< 信息等级
    Warning,    ///< 警告等级
    Error,      ///< 错误等级
    Fatal       ///< 致命错误等级（如果是这个等级，在输出了日志后会终止程序）
};

/**
 * 日志等级转字符串
 * @param level 日志等级，从 @ref emdevif::LogLevel 中选择
 * @return 该等级对应的字符串
 */
export constexpr const char* logLevelToStr(const LogLevel level) noexcept
{
    constexpr const char* const level_list[] = {"Debug", "Info", "Warning", "Error", "Fatal"};
    return level_list[static_cast<std::size_t>(level)];
}

#ifndef EMDEVIF_LOGGER_IGNORE_LEVEL
#define EMDEVIF_LOGGER_IGNORE_LEVEL LogLevel::Debug
#endif

static_assert(
    requires { static_cast<LogLevel>(EMDEVIF_LOGGER_IGNORE_LEVEL); },
    "EMDEVIF_LOGGER_IGNORE_LEVEL should be defined in `enum class LogLevel\' with it's style similar to "
    "`LogLevel::Debug\'");

/**
 * 日志忽略等级
 *
 * 日志等级由低到高依次为：`LogLevel::Debug`, `LogLevel::Info`, `LogLevel::Warning`, `LogLevel::Error`,
 * `LogLevel::Fatal`。@n 如果日志等级低于这个忽略等级，那么该日志将会被忽略掉。例如设置的忽略等级是 @n
 * `LogLevel::Warning`，那么 `LogLevel::Debug`和`LogLevel::Info` 会被忽略，其他的会被输出。
 */
export constexpr auto logger_ignore_level = static_cast<LogLevel>(EMDEVIF_LOGGER_IGNORE_LEVEL);

/// 日志异步模式下打印线程的栈大小
export constexpr auto logger_async_thread_stack_size = static_cast<std::size_t>(EMDEVIF_LOGGER_ASYNC_THREAD_STACK_SIZE);

/**
 * 日志类
 * @note 是单例模式，通过 Logger::getInstance() 获取实例。
 */
export class Logger
{
public:
    Logger(const Logger&) = delete;

    static Logger& getInstance() noexcept
    {
        static Logger instance;
        return instance;
    }

    using VSPrintfFunction = int (*)(char* dst, const char* format, std::va_list args);

    /**
     * 注册 vsprintf 函数，用于格式化输出。
     * @param function vsprintf 函数
     */
    static void registerVSPrintfFunction(const VSPrintfFunction function) noexcept
    {
        getInstance().vsprintf_ = function;
    }

private:
    ErrorCode printMsgToBuffer(const char* file,
                               const int line,
                               const char* func_name,
                               const LogLevel level,
                               const char* format,
                               std::va_list args) noexcept
    {
        print_msg_target_buffer_head_ = 0;

        auto len = sprintf_(getPrintMsgTargetBuffer(), "%-10zu [%s] ", getTimeLine(), logLevelToStr(level));
        if (len < 0) {
            return ErrorCode::OperationFail;
        }
        print_msg_target_buffer_head_ += len;

        len = vsprintf_(getPrintMsgTargetBuffer() + print_msg_target_buffer_head_, format, args);

        if (len < 0) {
            return ErrorCode::OperationFail;
        }
        print_msg_target_buffer_head_ += len;

        len = sprintf_(getPrintMsgTargetBuffer() + print_msg_target_buffer_head_,
                       " (at %s:%d in function `%s\')" EMDEVIF_LINE_SEPARATOR,
                       file,
                       line,
                       func_name);
        if (len < 0) {
            return ErrorCode::OperationFail;
        }

        return ErrorCode::Success;
    }

public:
    void log(const char* file,
             const int line,
             const char* func_name,
             const LogLevel level,
             const char* format,
             ...) noexcept
    {
        if (level < logger_ignore_level) {
            return;
        }

        std::va_list args;
        va_start(args, format);

        lock();

#if (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)

        if (printMsgToBuffer(file, line, func_name, level, format, args) == ErrorCode::Success) {
            printLogMessage(getPrintMsgTargetBuffer());
        }

        if (level == LogLevel::Fatal) {
            terminate();
        }

#else   // (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)

        if (printMsgToBuffer(file, line, func_name, level, format, args) == ErrorCode::Success) {
            if (level == LogLevel::Fatal) {
                printLogMessage(getPrintMsgTargetBuffer());
                terminate();
            }

            switchToNextSlot();
            logger_async_printer_semaphore_.release(false);
        }

#endif  // (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)

        unlock();

        va_end(args);
    }

private:
    Logger() noexcept = default;
    ~Logger() noexcept = default;

    VSPrintfFunction vsprintf_{std::vsprintf};

    int sprintf_(char* dst, const char* format, ...) const noexcept
    {
        va_list args;
        va_start(args, format);
        const auto ret = vsprintf_(dst, format, args);
        va_end(args);
        return ret;
    }

    static std::size_t getTimeLine() noexcept
    {
        return ::emdevif::user_declares::logger::getTimeLine();
    }

    static ErrorCode printLogMessage(const char* msg) noexcept
    {
        return ::emdevif::user_declares::logger::printLogMessage(msg);
    }

    void lock() noexcept  // NOLINT
    {
#if (EMDEVIF_LOGGER_USE_MUTEX)
        mutex_.lock();
#endif
    }

    void unlock() noexcept  // NOLINT
    {
#if (EMDEVIF_LOGGER_USE_MUTEX)
        mutex_.unlock();
#endif
    }

public:
    static void init() noexcept
    {
        getInstance().initImpl();
    }

private:
    void initImpl() noexcept
    {
#if (EMDEVIF_LOGGER_USE_MUTEX)
        mutex_ = Mutex::create({.name = "loggerMutex", .static_instance = &mutex_static_instance_});
        if (mutex_.getHandle() == nullptr) {
            EMDEVIF_FATAL_HANDLER("Failed to create logger mutex");
        }
#endif

#if (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_ASYNC)
        logger_async_printer_semaphore_ = BinarySemaphore::create(
            {.name = "loggerSemaphore",
             .static_instance = &logger_async_printer_semaphore_static_instance_});
        if (logger_async_printer_semaphore_.getHandle() == nullptr) {
            EMDEVIF_FATAL_HANDLER("Failed to create logger semaphore");
        }

        logger_async_printer_thread_ =
            Thread::create({.name = "loggerThread",
                            .priority = Thread::Priority::Low,
                            .static_instance = logger_async_printer_thread_instance_.getInstanceAddr(),
                            .stack_size = logger_async_printer_thread_instance_.getStackDepth()},
                           logPrinterThread,
                           this);
        if (logger_async_printer_thread_.getHandle() == nullptr) {
            EMDEVIF_FATAL_HANDLER("Failed to create logger async printer thread");
        }
#endif
    }

#if (EMDEVIF_LOGGER_USE_MUTEX)
private:
    Mutex mutex_{};
    Mutex::StaticInstance mutex_static_instance_{};
#endif

#if (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)
private:
    char buffer_[logger_buffer_size * logger_buffer_count]{};

    std::size_t print_msg_target_buffer_head_ = 0;

    char* getPrintMsgTargetBuffer() noexcept
    {
        return buffer_;
    }

#else   // (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)
private:
    RingBuffer<std::array<char, logger_buffer_size>, logger_buffer_count> buffer_{};

    std::size_t print_msg_target_buffer_head_ = 0;

    char* getPrintMsgTargetBuffer() noexcept
    {
        return &buffer_.nextSlot()[0];
    }

    void switchToNextSlot() noexcept
    {
        buffer_.useNextSlot();
    }

    Thread logger_async_printer_thread_{};
    Thread::StaticInstance<logger_async_thread_stack_size> logger_async_printer_thread_instance_{};

    BinarySemaphore logger_async_printer_semaphore_{};
    BinarySemaphore::StaticInstance logger_async_printer_semaphore_static_instance_{};

    EMDEVIF_NO_RETURN static void logPrinterThread(void* arguments) noexcept
    {
        auto& logger = *static_cast<Logger*>(arguments);

        while (true) {
            logger.logger_async_printer_semaphore_.acquire(false);

            logger.lock();

            if (!logger.buffer_.isEmpty()) {
                if (printLogMessage(&logger.buffer_.peekRef()[0]) == ErrorCode::Success) {
                    logger.buffer_.discard(1);
                }

                logger.unlock();
                logger.logger_async_printer_semaphore_.release(false);

                Thread::yield();
            }
            else {
                logger.unlock();
            }
        }
    }

#endif  // (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)
};

}  // namespace emdevif
