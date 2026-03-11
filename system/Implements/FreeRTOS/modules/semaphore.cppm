/**
 * @file semaphore.cppm
 * @brief 信号量接口
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

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.system.semaphore;

import emdevif.core.error_handler;
import emdevif.system.thread;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/system/semaphore.hpp"
