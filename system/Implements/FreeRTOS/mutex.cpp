/**
 * @file mutex.cpp
 * @brief
 */

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module;
#else
#include "emdevif/system/mutex.hpp"

#include "emdevif/core/error_handler.hpp"
#endif

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#else
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#include "emdevif/core/fatal_handler.h"

#include <cstddef>

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module emdevif.sys.mutex;

import emdevif.core.error_handler;
#endif

namespace emdevif {

Mutex::StronglyTypedHandle Mutex::create(const Attribute& attribute) noexcept
{
    QueueHandle_t handle = nullptr;

    if (attribute.static_instance != nullptr) {
        auto& static_instance = *static_cast<Mutex::StaticInstance*>(attribute.static_instance);

        handle = xSemaphoreCreateMutexStatic(&static_cast<StaticSemaphore_t&>(static_instance));
    }
    else {
        handle = xSemaphoreCreateMutex();
    }

#if (configQUEUE_REGISTRY_SIZE > 0)
    if (handle != nullptr) {
        vQueueAddToRegistry(handle, attribute.name);
    }
#endif

    return {.value = handle};
}

void Mutex::destroy(Mutex& obj) noexcept
{
    if (obj.handle_ != nullptr) {
        vSemaphoreDelete(obj.handle_);

#if (configQUEUE_REGISTRY_SIZE > 0)
        vQueueUnregisterQueue(static_cast<QueueHandle_t>(obj.handle_));
#endif

        obj.handle_ = nullptr;
    }
}

Mutex::~Mutex() noexcept
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif
