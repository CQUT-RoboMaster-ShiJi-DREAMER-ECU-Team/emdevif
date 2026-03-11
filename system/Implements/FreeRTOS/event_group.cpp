/**
 * @file event_group.cpp
 * @brief
 */

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module;
#else
#include "emdevif/system/event_group.hpp"

#include "emdevif/core/error_handler.hpp"
#endif

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#else
#include "FreeRTOS.h"
#include "event_groups.h"
#endif

#include <cstddef>

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module emdevif.system.event_group;

import emdevif.core.error_handler;
#endif

namespace emdevif {

EventGroup::StronglyTypedHandle EventGroup::create(const Attribute& attribute)
{
    Handle handle;

    if (attribute.static_instance != nullptr) {
        auto& static_instance = static_cast<StaticEventGroup_t&>(*attribute.static_instance);

        handle = xEventGroupCreateStatic(&static_instance);
    }
    else {
        handle = xEventGroupCreate();
    }

    return {handle};
}

void EventGroup::destroy(EventGroup& obj)
{
    if (obj.handle_ != nullptr) {
        vEventGroupDelete(static_cast<EventGroupHandle_t>(obj.handle_));

        obj.handle_ = nullptr;
    }
}

EventGroup::~EventGroup()
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif
