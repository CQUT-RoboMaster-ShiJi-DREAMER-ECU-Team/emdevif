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

namespace emdevif {

EMDEVIF_MODULE_EXPORT
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

EMDEVIF_MODULE_EXPORT  // ReSharper disable once CppRedundantCastExpression
    constexpr auto logger_mode = static_cast<LoggerMode>(EMDEVIF_LOGGER_MODE);

EMDEVIF_MODULE_EXPORT
enum class LoggerLevel : int_fast8_t {
    Verbose = 0,
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

#ifndef EMDEVIF_LOGGER_IGNORE_LEVEL
#define EMDEVIF_LOGGER_IGNORE_LEVEL ::emdevif::LoggerLevel::Info
#endif

EMDEVIF_MODULE_EXPORT  // ReSharper disable once CppRedundantCastExpression
    constexpr auto logger_ignore_level = static_cast<LoggerLevel>(EMDEVIF_LOGGER_IGNORE_LEVEL);

static_assert(logger_ignore_level >= LoggerLevel::Verbose && logger_ignore_level <= LoggerLevel::Fatal,
              "Invalid EMDEVIF_LOGGER_IGNORE_LEVEL value! It should be a value in enum class `emdevif::LoggerLevel`.");

}  // namespace emdevif

#endif  // !EMDEVIF_LOGGER_CONFIG_HPP
