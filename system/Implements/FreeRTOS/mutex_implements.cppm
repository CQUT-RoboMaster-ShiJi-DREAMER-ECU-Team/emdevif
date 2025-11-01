/**
 * @file mutex_implements.cppm
 * @author DuYicheng
 * @date 2025-08-01
 * @brief 互斥锁 FreeRTOS 实现
 */

// ReSharper disable CppMemberFunctionMayBeConst

module;

#include <cstddef>

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#else
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#include "emdevif/fatal_handler.h"

export module emdevif.sys.mutex:implements;
import :interface;

import emdevif.errorHandler;

export namespace emdevif {

class Mutex::StaticInstance
{
public:
    StaticInstance() noexcept : instance() {}

    friend class Mutex;

private:
    explicit operator StaticSemaphore_t&() noexcept
    {
        return instance;
    }

private:
    StaticSemaphore_t instance;
};

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
// NOLINT
{
    if (obj.handle_ != nullptr) {
        vSemaphoreDelete(obj.handle_);

#if (configQUEUE_REGISTRY_SIZE > 0)
        vQueueUnregisterQueue(static_cast<QueueHandle_t>(obj.handle_));
#endif

        obj.handle_ = nullptr;
    }
}

inline ErrorCode Mutex::lock(const SysTick_t timeout_tick) noexcept
{
    const auto ret = xSemaphoreTake(static_cast<SemaphoreHandle_t>(handle_), timeout_tick);

    if (ret == pdTRUE) {
        return ErrorCode::Success;
    }
    else {
        return ErrorCode::Timeout;
    }
}

inline void Mutex::unlock() noexcept
{
    if (xSemaphoreGive(handle_) != pdTRUE) {
        EMDEVIF_FATAL_HANDLER("Failed to Unlock mutex!");
    }
}

Mutex::~Mutex() noexcept
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif
