/**
 * @file thread.cppm
 * @brief 线程接口
 */

module;

#include "emdevif/core/attributes_and_useful_macros.h"
#include "emdevif/core/fatal_handler.h"

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include <cassert>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#else
#include "FreeRTOS.h"
#include "task.h"
#endif

#include <cstdint>

#include <tuple>
#include <utility>
#include <type_traits>
#include <limits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.sys.thread;

import emdevif.core.error_handler;
import emdevif.sys.heap;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/system/thread.hpp"
