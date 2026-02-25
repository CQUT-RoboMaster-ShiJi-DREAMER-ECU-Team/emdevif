/**
 * @file config.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_LOGGER_CONFIG_HPP
#define EMDEVIF_LOGGER_CONFIG_HPP

#include "emdevif/core/detail/config.hpp"

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#include <cstdint>
#endif

EMDEVIF_MODULE_EXPORT
namespace emdevif::logger {

enum class LoggerMode : uint_fast8_t {
    Sync = 0,
    Async = 1,
    ExternalImpl = 2
};

#ifndef EMDEVIF_LOGGER_MODE
#define EMDEVIF_LOGGER_MODE 0
#endif

#if (!((EMDEVIF_LOGGER_MODE) >= 0 && (EMDEVIF_LOGGER_MODE) <= 2))
#error "Invalid EMDEVIF_LOGGER_MODE value! It should be 0 (Sync), 1 (Async) or 2 (ExternalImpl)."
#endif

// ReSharper disable once CppRedundantCastExpression
constexpr auto logger_mode = static_cast<LoggerMode>(EMDEVIF_LOGGER_MODE);

enum class LoggerLevel : int_fast8_t {
    Verbose = 0,
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

#ifndef EMDEVIF_LOGGER_IGNORE_LEVEL
#define EMDEVIF_LOGGER_IGNORE_LEVEL ::emdevif::logger::LoggerLevel::Info
#endif

// ReSharper disable once CppRedundantCastExpression
constexpr auto logger_ignore_level = static_cast<LoggerLevel>(EMDEVIF_LOGGER_IGNORE_LEVEL);

static_assert(logger_ignore_level >= LoggerLevel::Verbose && logger_ignore_level <= LoggerLevel::Fatal,
              "Invalid EMDEVIF_LOGGER_IGNORE_LEVEL value! It should be a value in enum class `emdevif::LoggerLevel`.");

constexpr const char* toCString(const LoggerLevel logger_level) noexcept
{
    switch (logger_level) {
    case LoggerLevel::Verbose:
        return "Verbose";
    case LoggerLevel::Debug:
        return "Debug";
    case LoggerLevel::Info:
        return "Info";
    case LoggerLevel::Warning:
        return "Warning";
    case LoggerLevel::Error:
        return "Error";
    case LoggerLevel::Fatal:
        return "Fatal";
    default:
        return "Unknown";
    }
}

/* clang-format off */
#if (EMDEVIF_LOGGER_MODE == 0)  // Sync
    #ifndef EMDEVIF_LOGGER_BUFFER_SIZE
        #define EMDEVIF_LOGGER_BUFFER_SIZE 256U
    #endif

    #ifndef EMDEVIF_LOGGER_BUFFER_COUNT
        #define EMDEVIF_LOGGER_BUFFER_COUNT 4
    #endif

    #ifndef EMDEVIF_LOGGER_SYNC_USE_LOCK
        #define EMDEVIF_LOGGER_SYNC_USE_LOCK true
    #endif

    #ifdef EMDEVIF_LOGGER_ASYNC_THREAD_STACK_SIZE
        #warning "EMDEVIF_LOGGER_ASYNC_THREAD_STACK_SIZE is defined but will be ignored because the logger mode is Sync."
    #endif

    #ifndef EMDEVIF_LOGGER_DYNAMIC_CREATE
        #define EMDEVIF_LOGGER_DYNAMIC_CREATE false
    #endif
#elif (EMDEVIF_LOGGER_MODE == 1)  // Async
    #ifndef EMDEVIF_LOGGER_BUFFER_SIZE
        #define EMDEVIF_LOGGER_BUFFER_SIZE 256U
    #endif

    #ifndef EMDEVIF_LOGGER_BUFFER_COUNT
        #define EMDEVIF_LOGGER_BUFFER_COUNT 4
    #endif

    #ifdef EMDEVIF_LOGGER_SYNC_USE_LOCK
        #warning "EMDEVIF_LOGGER_SYNC_USE_LOCK is defined but will be ignored because the logger mode is Async."
    #endif

    #ifndef EMDEVIF_LOGGER_ASYNC_THREAD_STACK_SIZE
        #define EMDEVIF_LOGGER_ASYNC_THREAD_STACK_SIZE 128
    #endif

    #ifndef EMDEVIF_LOGGER_DYNAMIC_CREATE
        #define EMDEVIF_LOGGER_DYNAMIC_CREATE false
    #endif
#else  // ExternalImpl
    #ifdef EMDEVIF_LOGGER_BUFFER_SIZE
        #warning "EMDEVIF_LOGGER_BUFFER_SIZE is defined but will be ignored because the logger mode is ExternalImpl."
    #endif

    #ifdef EMDEVIF_LOGGER_BUFFER_COUNT
        #warning "EMDEVIF_LOGGER_BUFFER_COUNT is defined but will be ignored because the logger mode is ExternalImpl."
    #endif

    #ifdef EMDEVIF_LOGGER_ASYNC_THREAD_STACK_SIZE
        #warning \
            "EMDEVIF_LOGGER_ASYNC_THREAD_STACK_SIZE is defined but will be ignored because the logger mode is ExternalImpl."
    #endif

    #ifdef EMDEVIF_LOGGER_SYNC_USE_LOCK
        #warning "EMDEVIF_LOGGER_SYNC_USE_LOCK is defined but will be ignored because the logger mode is ExternalImpl."
    #endif

    #ifdef EMDEVIF_LOGGER_DYNAMIC_CREATE
        #warning "EMDEVIF_LOGGER_DYNAMIC_CREATE is defined but will be ignored because the logger mode is ExternalImpl."
    #endif
#endif
/* clang-format on */

// ReSharper disable once CppRedundantCastExpression
constexpr auto logger_buffer_size = static_cast<std::size_t>(EMDEVIF_LOGGER_BUFFER_SIZE);

// ReSharper disable once CppRedundantCastExpression
constexpr auto logger_buffer_count = static_cast<std::size_t>(EMDEVIF_LOGGER_BUFFER_COUNT);

}  // namespace emdevif::logger

#endif  // !EMDEVIF_LOGGER_CONFIG_HPP
