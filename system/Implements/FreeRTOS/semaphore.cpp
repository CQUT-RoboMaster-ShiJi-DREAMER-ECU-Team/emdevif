/**
 * @file semaphore.cpp
 * @brief
 */

#if (defined(EMDEVIF_USE_MODULES) && EMDEVIF_USE_MODULES)
module;
#else
#include "emdevif/system/semaphore.hpp"

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
module emdevif.sys.semaphore;

import emdevif.core.error_handler;
#endif

namespace emdevif {

CountingSemaphore<1>::StronglyTypedHandle CountingSemaphore<1>::create(const Attribute& attribute) noexcept
{
    Handle handle = nullptr;

    if (attribute.static_instance != nullptr) {
        auto& static_instance = *static_cast<CountingSemaphore<1>::StaticInstance*>(attribute.static_instance);

        handle = xSemaphoreCreateBinaryStatic(&static_cast<StaticSemaphore_t&>(static_instance));
    }
    else {
        handle = xSemaphoreCreateBinary();
    }

#if (configQUEUE_REGISTRY_SIZE > 0)
    if (handle != nullptr) {
        vQueueAddToRegistry(static_cast<QueueHandle_t>(handle), attribute.name);
    }
#endif

    return {handle};
}

void CountingSemaphore<1>::destroy(CountingSemaphore<1>& obj) noexcept
{
    if (obj.handle_ != nullptr) {
        vSemaphoreDelete(obj.handle_);
        obj.handle_ = nullptr;
    }
}

CountingSemaphore<1>::~CountingSemaphore() noexcept
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif
