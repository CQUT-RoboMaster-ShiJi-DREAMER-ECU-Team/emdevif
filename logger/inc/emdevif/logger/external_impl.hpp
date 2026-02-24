/**
 * @file external_impl.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_LOGGER_EXTERNAL_IMPL_HPP
#define EMDEVIF_LOGGER_EXTERNAL_IMPL_HPP

#include "emdevif/logger/config.hpp"

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#include "emdevif/core/fatal_handler.h"
#include "emdevif/core/error_handler.hpp"

#include "emdevif/core/detail/include_emdevif_user_declares_in_headers.hpp"

#include <cstdarg>
#endif

EMDEVIF_MODULE_EXPORT
namespace emdevif::logger {

template<typename... Args>
emdevif::ErrorCode init(Args&&... args) noexcept
{
    return ::emdevif::user_declares::logger::logInit(std::forward<Args>(args)...);
}

inline void deInit() noexcept
{
    ::emdevif::user_declares::logger::logDeInit();
}

inline void log(const LoggerLevel level, const char* format, ...) noexcept
{
    if (level < logger_ignore_level) {
        return;
    }

    std::va_list args;
    va_start(args, format);
    ::emdevif::user_declares::logger::logImpl(static_cast<int>(level), format, args);
    va_end(args);
}

inline void verbose(const char* format, ...) noexcept
{
    // ReSharper disable once CppRedundantComplexityInComparison
    if constexpr (constexpr auto level = LoggerLevel::Verbose; !(level < logger_ignore_level)) {
        std::va_list args;
        va_start(args, format);
        ::emdevif::user_declares::logger::logVerboseImpl(static_cast<int>(level), format, args);
        va_end(args);
    }
}

inline void debug(const char* format, ...) noexcept
{
    // ReSharper disable once CppRedundantComplexityInComparison
    if constexpr (constexpr auto level = LoggerLevel::Debug; !(level < logger_ignore_level)) {
        std::va_list args;
        va_start(args, format);
        ::emdevif::user_declares::logger::logDebugImpl(static_cast<int>(level), format, args);
        va_end(args);
    }
}

inline void info(const char* format, ...) noexcept
{
    // ReSharper disable once CppRedundantComplexityInComparison
    if constexpr (constexpr auto level = LoggerLevel::Info; !(level < logger_ignore_level)) {
        std::va_list args;
        va_start(args, format);
        ::emdevif::user_declares::logger::logInfoImpl(static_cast<int>(level), format, args);
        va_end(args);
    }
}

inline void warning(const char* format, ...) noexcept
{
    // ReSharper disable once CppRedundantComplexityInComparison
    if constexpr (constexpr auto level = LoggerLevel::Warning; !(level < logger_ignore_level)) {
        std::va_list args;
        va_start(args, format);
        ::emdevif::user_declares::logger::logWarningImpl(static_cast<int>(level), format, args);
        va_end(args);
    }
}

inline void error(const char* format, ...) noexcept
{
    // ReSharper disable once CppRedundantComplexityInComparison
    if constexpr (constexpr auto level = LoggerLevel::Error; !(level < logger_ignore_level)) {
        std::va_list args;
        va_start(args, format);
        ::emdevif::user_declares::logger::logErrorImpl(static_cast<int>(level), format, args);
        va_end(args);
    }
}

inline void fatal(const char* format, ...) noexcept
{
    // ReSharper disable once CppRedundantComplexityInComparison
    if constexpr (constexpr auto level = LoggerLevel::Fatal; !(level < logger_ignore_level)) {
        std::va_list args;
        va_start(args, format);
        ::emdevif::user_declares::logger::logFatalImpl(static_cast<int>(level), format, args);
        va_end(args);

        EMDEVIF_FATAL_HANDLER("Fatal log message outputted, terminating program");
    }
}

}  // namespace emdevif::logger

#endif  // !EMDEVIF_LOGGER_EXTERNAL_IMPL_HPP
