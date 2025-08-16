/**
 * @file event_group_implements.cppm
 * @module emdevif.sys.event_group:implements
 * @author DuYicheng
 * @date 2025-08-16
 * @brief 事件组 FreeRTOS 实现
 */

module;

#include "FreeRTOS.h"
#include "event_groups.h"

export module emdevif.sys.event_group:implements;
import :interface;

export namespace emdevif {

consteval auto EventGroup::maxEventBits() noexcept
{
    using return_type = uint_fast8_t;

#if (configUSE_16_BIT_TICKS)
    return static_cast<return_type>(8);
#else
    return static_cast<return_type>(24);
#endif
}

}  // namespace emdevif
