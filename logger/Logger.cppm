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

#include <string_view>

export module emdevif.logger;

import emdevif.userDeclares;
import emdevif.util.ringBuffer;

namespace emdevif {

#ifndef EMDEVIF_LOGGER_BUFFER_SIZE
#define EMDEVIF_LOGGER_BUFFER_SIZE 256U
#endif

#define EMDEVIF_LOGGER_MODE_SYNC  0
#define EMDEVIF_LOGGER_MODE_ASYNC 1

#ifndef EMDEVIF_LOGGER_MODE
#define EMDEVIF_LOGGER_MODE EMDEVIF_LOGGER_MODE_SYNC
#endif

#ifndef EMDEVIF_LOGGER_SYNC_USE_LOCK
#define EMDEVIF_LOGGER_SYNC_USE_LOCK true
#endif

constexpr std::size_t logger_buffer_size = EMDEVIF_LOGGER_BUFFER_SIZE;

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

constexpr auto logger_ignore_level = static_cast<LogLevel>(EMDEVIF_LOGGER_IGNORE_LEVEL);

export class Logger
{
public:
    Logger(const Logger&) = delete;

    static Logger& getInstance() noexcept
    {
        static Logger instance;
        return instance;
    }

    using VSPrintfFunction = int (*)(char* dst, const char* format, std::va_list);

    void registerVSPrintfFunction(const VSPrintfFunction function) noexcept
    {
        vsprintf_ = function;
    }

    void log(const char* file,
             const int line,
             const char* func_name,
             const LogLevel level,
             const char* format,
             ...) noexcept
    {
        if (level <= logger_ignore_level) {
            return;
        }

#if (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)
        std::va_list args;
        va_start(args, format);

        lock();
        [&]() noexcept {
            buffer_head_ = 0;

            auto len = sprintf_(getBuffer(), "%-10zu [%s] ", getTimeLine(), logLevelToStr(level));
            if (len < 0) {
                // what can I say?
                return;
            }
            buffer_head_ += len;

            len = vsprintf_(getBuffer() + buffer_head_, format, args);

            if (len < 0) {
                return;
            }
            buffer_head_ += len;

            len = sprintf_(getBuffer() + buffer_head_, " (at %s:%d in function `%s\')\r\n", file, line, func_name);
            if (len < 0) {
                return;
            }
        }();
        unlock();
        va_end(args);
#else   // (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)
#endif  // (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)
    }

private:
    Logger() noexcept : vsprintf_(std::vsprintf), buffer_() {}
    ~Logger() noexcept = default;

    VSPrintfFunction vsprintf_;

    RingBuffer<char, logger_buffer_size> buffer_;

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

#if (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)

    std::size_t buffer_head_ = 0;

    char* getBuffer() noexcept
    {
        return &buffer_.peekRef();
    }

    static void lock() noexcept
    {
#if (EMDEVIF_LOGGER_SYNC_USE_LOCK)
        ::emdevif::user_declares::logger::lock();
#endif
    }

    static void unlock() noexcept
    {
#if (EMDEVIF_LOGGER_SYNC_USE_LOCK)
        ::emdevif::user_declares::logger::unlock();
#endif
    }

#endif  // (EMDEVIF_LOGGER_MODE == EMDEVIF_LOGGER_MODE_SYNC)
};

}  // namespace emdevif
