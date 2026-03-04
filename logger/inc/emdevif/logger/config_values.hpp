/**
 * @file config_values.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_LOGGER_CONFIG_VALUES_HPP
    #define EMDEVIF_LOGGER_CONFIG_VALUES_HPP

    #include "emdevif/logger/config.hpp"

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

    #if (EMDEVIF_LOGGER_MODE == 0 || EMDEVIF_LOGGER_MODE == 1)
// ReSharper disable once CppRedundantCastExpression
constexpr auto logger_buffer_size = static_cast<std::size_t>(EMDEVIF_LOGGER_BUFFER_SIZE);

// ReSharper disable once CppRedundantCastExpression
constexpr auto logger_buffer_count = static_cast<std::size_t>(EMDEVIF_LOGGER_BUFFER_COUNT);
    #endif

}  // namespace emdevif::logger

#endif  // !EMDEVIF_LOGGER_CONFIG_VALUES_HPP
