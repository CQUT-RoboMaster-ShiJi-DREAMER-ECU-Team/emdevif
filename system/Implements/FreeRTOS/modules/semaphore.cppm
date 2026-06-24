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

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.system.semaphore;

#include "emdevif/system/semaphore.hpp"

export namespace emdevif {
using ::emdevif::CountingSemaphoreStaticInstance;
using ::emdevif::CountingSemaphoreBuilder;
using ::emdevif::CountingSemaphore;
using ::emdevif::BinarySemaphore;
}
