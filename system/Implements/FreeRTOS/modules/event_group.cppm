/**
 * @file event_group.cppm
 * @brief 事件组接口
 */

module;

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#else
#include "FreeRTOS.h"
#include "event_groups.h"
#endif

#include "emdevif/core/fatal_handler.h"

#include <cstdint>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.sys.eventGroup;

import emdevif.core.error_handler;
import emdevif.sys.thread;
import emdevif.util.bitInt;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/system/event_group.hpp"
