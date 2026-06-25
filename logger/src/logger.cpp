/**
 * @file logger.cpp
 * @brief
 */

#include "emdevif/logger/config.hpp"

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module;
#else
    #include "emdevif/core/error_handler.hpp"
    #include "emdevif/logger.hpp"

    #if (EMDEVIF_LOGGER_MODE == 0)
        // Sync
        #if (defined(EMDEVIF_LOGGER_SYNC_USE_LOCK) && EMDEVIF_LOGGER_SYNC_USE_LOCK)
            #include "emdevif/core/resource_guard/lock_guard.hpp"
            #include "emdevif/system/mutex.hpp"
        #endif
    #else
        // Async
        #include "emdevif/core/data_container/ring_buffer.hpp"
        #include "emdevif/core/resource_guard/lock_guard.hpp"
        #include "emdevif/system/mutex.hpp"
        #include "emdevif/system/semaphore.hpp"
        #include "emdevif/system/thread.hpp"
    #endif
#endif

#ifndef EMDEVIF_USER_DECLARES_PROVIDE_MODULE
    #include "emdevif/user_declares.hpp"
#endif

#include <cstdarg>
#include <cstdint>

#include <iterator>

#include "emdevif/core/attributes_and_useful_macros.h"
#include "emdevif/core/fatal_handler.h"
#include "emdevif/core/line_separator.h"

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module emdevif.logger;

import emdevif.core.error_handler;
#endif

#ifdef EMDEVIF_USER_DECLARES_PROVIDE_MODULE
import emdevif.user_declares;
#endif

#if (EMDEVIF_LOGGER_MODE == 0)
    // Sync
    #if (defined(EMDEVIF_LOGGER_SYNC_USE_LOCK) && EMDEVIF_LOGGER_SYNC_USE_LOCK)
        #if EMDEVIF_USE_MODULES
import emdevif.core.resource_guard.lock_guard;
import emdevif.system.mutex;
        #else
            #include "emdevif/core/resource_guard/lock_guard.hpp"
            #include "emdevif/system/mutex.hpp"
        #endif
    #endif
#else
    // Async
    #if EMDEVIF_USE_MODULES
import emdevif.core.data_container.ring_buffer;
import emdevif.core.resource_guard.lock_guard;
import emdevif.system.mutex;
import emdevif.system.semaphore;
import emdevif.system.thread;
    #else
        #include "emdevif/core/data_container/ring_buffer.hpp"
        #include "emdevif/core/resource_guard/lock_guard.hpp"
        #include "emdevif/system/mutex.hpp"
        #include "emdevif/system/semaphore.hpp"
        #include "emdevif/system/thread.hpp"
    #endif
#endif

#if (EMDEVIF_LOGGER_MODE != 2)  // 不是 ExternalImpl 模式的情况
namespace emdevif::logger::detail {

static VsnprintfImpl vsnprintf_impl_ = nullptr;

static int snprintf_(char* dest, const std::size_t buffer_size, const char* format, ...) noexcept
{
    std::va_list args;
    va_start(args, format);
    const auto ret = vsnprintf_impl_(dest, buffer_size, format, args);
    va_end(args);
    return ret;
}

}  // namespace emdevif::logger::detail
#endif  // (EMDEVIF_LOGGER_MODE != 2)

#if (EMDEVIF_LOGGER_MODE == 0)
    #include "./sync.hpp"
#elif (EMDEVIF_LOGGER_MODE == 1)
    #include "./async.hpp"
#elif (EMDEVIF_LOGGER_MODE == 2)
// Nothing to include, the implementation is provided by user.
#endif
