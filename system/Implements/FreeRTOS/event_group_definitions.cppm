/**
 * @file event_group_definitions.cppm
 * @module emdevif.sys.event_group:definitions
 * @author DuYicheng
 * @date 2025-08-29
 * @brief 事件组相关常量定义 - FreeRTOS 实现
 */

module;

#include "FreeRTOS.h"
#include "event_groups.h"

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
