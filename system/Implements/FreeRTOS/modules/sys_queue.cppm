/**
 * @file sys_queue.cppm
 * @brief 系统队列
 */

module;

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#else
#include "FreeRTOS.h"
#include "queue.h"
#endif

#include "emdevif/core/fatal_handler.h"

#include <cstdint>
#include <cstddef>

#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.system.sys_queue;

import emdevif.core.error_handler;
export import emdevif.core.data_container.message_queue;
import emdevif.system.thread;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/system/sys_queue.hpp"
