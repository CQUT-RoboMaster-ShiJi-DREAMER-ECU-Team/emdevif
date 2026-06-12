/**
 * @file mutex.cppm
 * @brief 互斥锁
 */

module;

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
    #include "freertos/FreeRTOS.h"
    #include "freertos/semphr.h"
#else
    #include "FreeRTOS.h"
    #include "semphr.h"
#endif

#include "emdevif/core/fatal_handler.h"

#include <cstddef>
#include <cstdint>
#include <utility>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.system.mutex;

import emdevif.core.error_handler;
import emdevif.system.thread;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/system/mutex.hpp"
