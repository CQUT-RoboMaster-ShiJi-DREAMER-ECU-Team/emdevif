/**
 * @file mutex_implements.cppm
 * @module emdevif.sys.mutex:implements
 * @author DuYicheng
 * @date 2025-08-01
 * @brief 互斥锁 FreeRTOS 实现
 */

// ReSharper disable CppMemberFunctionMayBeConst

module;

#include <cstddef>

#include "FreeRTOS.h"
#include "semphr.h"

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

    std::size_t getInstanceSize() noexcept
    {
        return sizeof instance;
    }

private:
    explicit operator StaticSemaphore_t&() noexcept
    {
        return instance;
    }

private:
    StaticSemaphore_t instance;
};

Mutex::StronglyTypedHandle Mutex::create(const Attribute& attribute)
{
    QueueHandle_t handle = nullptr;

    if (attribute.static_instance != nullptr && attribute.instance_size != 0U) {
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

void Mutex::destroy(Mutex& obj)  // NOLINT
{
    if (obj.handle_ != nullptr) {
        vSemaphoreDelete(obj.handle_);

#if (configQUEUE_REGISTRY_SIZE > 0)
        vQueueUnregisterQueue(static_cast<QueueHandle_t>(obj.handle_));
#endif

        obj.handle_ = nullptr;
    }
}

inline ErrorCode Mutex::lock(const SysTick_t timeout_tick)
{
    const auto ret = xSemaphoreTake(static_cast<SemaphoreHandle_t>(handle_), timeout_tick);

    if (ret == pdTRUE) {
        return ErrorCode::Success;
    }
    else {
        return ErrorCode::Timeout;
    }
}

inline void Mutex::unlock()
{
    if (xSemaphoreGive(handle_) != pdTRUE) {
        EMDEVIF_FATAL_HANDLER("Failed to Unlock mutex!");
    }
}

Mutex::~Mutex()
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif
