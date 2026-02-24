/**
 * @file sync.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_LOGGER_SYNC_HPP
#define EMDEVIF_LOGGER_SYNC_HPP

#include "emdevif/logger/config.hpp"

#ifndef EMDEVIF_USER_DECLARES_PROVIDE_MODULE
#include "emdevif/user_declares.hpp"
#endif

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#include "emdevif/core/fatal_handler.h"
#include "emdevif/core/error_handler.hpp"

#include <cstdarg>
#endif

namespace emdevif::logger::detail {

namespace sync {

using VsprintfImpl = int (*)(char* dest, const char* format, std::va_list args);

ErrorCode logInit(VsprintfImpl vsprintf_impl) noexcept;

}  // namespace sync

inline ErrorCode logInit(const sync::VsprintfImpl vsprintf_impl) noexcept
{
    return sync::logInit(vsprintf_impl);
}

inline void logDeInit() noexcept {}

inline void logImpl(const LoggerLevel level, const char* format, std::va_list args) noexcept {}

}  // namespace emdevif::logger::detail

#include "emdevif/logger/sync_async_interface.hpp"

#endif  // !EMDEVIF_LOGGER_SYNC_HPP
