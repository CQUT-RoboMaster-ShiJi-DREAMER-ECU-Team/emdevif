/**
 * @file event_group_definitions.cppm
 * @brief 事件组相关常量定义 - FreeRTOS 实现
 */

module;

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#else
#include "FreeRTOS.h"
#include "event_groups.h"
#endif

export module emdevif.sys.eventGroup:definitions;

import emdevif.util.bitInt;

namespace emdevif::internal {

consteval BitsType_t maxEventBits() noexcept
{
#if (configUSE_16_BIT_TICKS)
    return 8;
#else
    return 24;
#endif
}

}  // namespace emdevif::internal
