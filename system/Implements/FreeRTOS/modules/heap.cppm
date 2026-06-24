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

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.system.heap;

#include "emdevif/system/heap.hpp"

export namespace emdevif::heap {
using ::emdevif::heap::construct;
using ::emdevif::heap::destruct;
using ::emdevif::heap::Deleter;
using ::emdevif::heap::unique_ptr;
using ::emdevif::heap::make_unique;
using ::emdevif::heap::Allocator;
}
