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

#include <cstddef>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.system.mutex;

#include "emdevif/system/mutex.hpp"

export namespace emdevif {
using ::emdevif::MutexStaticInstance;
using ::emdevif::MutexBuilder;
using ::emdevif::Mutex;
}
