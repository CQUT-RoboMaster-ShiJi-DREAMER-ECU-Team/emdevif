/**
 * @file heap.cppm
 * @brief 堆内存
 */

module;

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include "freertos/FreeRTOS.h"
#else
#include "FreeRTOS.h"
#endif

#include <memory>
#include <utility>
#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.system.heap;

import emdevif.core.concepts;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/system/heap.hpp"
