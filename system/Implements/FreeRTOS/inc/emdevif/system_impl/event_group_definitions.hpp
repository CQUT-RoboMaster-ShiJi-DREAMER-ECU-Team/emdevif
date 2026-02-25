/**
 * @file event_group_definitions.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_FREERTOS_SYSTEM_IMPL_EVENT_GROUP_DEFINITIONS_HPP
#define EMDEVIF_FREERTOS_SYSTEM_IMPL_EVENT_GROUP_DEFINITIONS_HPP

#include "emdevif/core/detail/config.hpp"

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#else
#include "FreeRTOS.h"
#include "event_groups.h"
#endif

#include "emdevif/util/bit_int.hpp"

#include <cstdint>
#endif

namespace emdevif::detail {

consteval BitsType_t maxEventBits() noexcept
{
#if (configUSE_16_BIT_TICKS)
    return 8;
#else
    return 24;
#endif
}

}  // namespace emdevif::detail

#endif  // !EMDEVIF_FREERTOS_SYSTEM_IMPL_EVENT_GROUP_DEFINITIONS_HPP
