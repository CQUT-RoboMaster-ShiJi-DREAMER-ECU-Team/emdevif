/**
 * @file Logger.cppm
 * @module emdevif.logger
 * @author DuYicheng
 * @date 2025-09-17
 * @brief 日志
 */

module;

#include <cstdint>
#include <cstdio>
#include <cstdarg>

#include <array>

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

export module emdevif.logger;

import emdevif.userDeclares;
import emdevif.errorHandler;

#if (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_ASYNC)
import emdevif.util.ringBuffer;
#endif

namespace emdevif {

export enum class LoggerMode : uint_fast8_t {
    Sync = EMDEVIF_LOGGER_MODE_SYNC,
    Async = EMDEVIF_LOGGER_MODE_ASYNC
};

export constexpr auto logger_mode = static_cast<LoggerMode>(EMDEVIF_LOGGER_MODE);

export constexpr std::size_t logger_buffer_size = EMDEVIF_LOGGER_BUFFER_SIZE;
export constexpr std::size_t logger_buffer_count = EMDEVIF_LOGGER_BUFFER_COUNT;

export enum class LogLevel : uint_fast8_t {
    Debug = 0,
    Info,
    Warning,
    Error,
    Fatal
};

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

export constexpr auto logger_ignore_level = static_cast<LogLevel>(EMDEVIF_LOGGER_IGNORE_LEVEL);

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

    void registerVSPrintfFunction(const VSPrintfFunction function) noexcept
    {
        vsprintf_ = function;
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
                       " (at %s:%d in function `%s\')\r\n",
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

#else   // (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)

        if (printMsgToBuffer(file, line, func_name, level, format, args) == ErrorCode::Success) {
            switchToNextSlot();
        }

#endif  // (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)

        if (level == LogLevel::Fatal) {
            terminate();
        }

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

    static void printLogMessage(const char* msg) noexcept
    {
        ::emdevif::user_declares::logger::printLogMessage(msg);
    }

    static void lock() noexcept
    {
#if (EMDEVIF_LOGGER_SYNC_USE_LOCK || EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_ASYNC)
        ::emdevif::user_declares::logger::lock();
#endif
    }

    static void unlock() noexcept
    {
#if (EMDEVIF_LOGGER_SYNC_USE_LOCK || EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_ASYNC)
        ::emdevif::user_declares::logger::unlock();
#endif
    }

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

public:
    static void logPrinterThread(void* arguments) noexcept
    {
        auto& logger = *static_cast<Logger*>(arguments);

        while (true) {
            lock();
            printLogMessage(&logger.buffer_.peekRef()[0]);
            logger.buffer_.discard(1);
            unlock();

            Thread::delay(1);  // todo 需要除去对 Thread 的依赖
        }
    }

#endif  // (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)
};

}  // namespace emdevif
