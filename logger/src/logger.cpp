/**
 * @file logger.cpp
 * @brief
 */

#include "emdevif/logger/config.hpp"

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module;
#else
#include "emdevif/logger.hpp"

#if (defined(EMDEVIF_LOGGER_SYNC_USE_LOCK) && EMDEVIF_LOGGER_SYNC_USE_LOCK)
#include "emdevif/system/mutex.hpp"
#include "emdevif/core/resource_guard/lock_guard.hpp"
#include "emdevif/core/error_handler.hpp"
#endif

#endif

#ifndef EMDEVIF_USER_DECLARES_PROVIDE_MODULE
#include "emdevif/user_declares.hpp"
#endif

#include "emdevif/core/line_separator.h"
#include "emdevif/core/fatal_handler.h"
#include "emdevif/core/attributes_and_useful_macros.h"

#include <cstdarg>

#include <iterator>

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module emdevif.logger;

import emdevif.core.error_handler;
#endif

#ifdef EMDEVIF_USER_DECLARES_PROVIDE_MODULE
import emdevif.userDeclares;
#endif

#if (defined(EMDEVIF_LOGGER_SYNC_USE_LOCK) && EMDEVIF_LOGGER_SYNC_USE_LOCK)
import emdevif.sys.mutex;
import emdevif.core.resource_guard.lock_guard;
#endif

namespace emdevif::logger::detail {

static VsnprintfImpl vsnprintf_impl_ = nullptr;

static char log_msg_buffer_[logger_buffer_count][logger_buffer_size];

static constexpr std::size_t logMsgBufferLength() noexcept
{
    return logger_buffer_count * logger_buffer_size;
}

static int snprintf_(char* dest, const std::size_t buffer_size, const char* format, ...) noexcept
{
    std::va_list args;
    va_start(args, format);
    const auto ret = vsnprintf_impl_(dest, buffer_size, format, args);
    va_end(args);
    return ret;
}

}  // namespace emdevif::logger::detail

#if (EMDEVIF_LOGGER_MODE == 0)
#include "./sync.hpp"
#elif (EMDEVIF_LOGGER_MODE == 1)

#elif (EMDEVIF_LOGGER_MODE == 2)
// Nothing to include, the implementation is provided by user.
#endif
