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

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.system.event_group;

#include "emdevif/system/event_group.hpp"

export namespace emdevif {
using ::emdevif::EventGroupStaticInstance;
using ::emdevif::EventGroupBuilder;
using ::emdevif::EventGroup;
}
