/**
 * @file mutex_implements.cppm
 * @module emdevif.mutex:implements
 * @author DuYicheng
 * @date 2025-08-01
 * @brief 互斥锁 FreeRTOS 实现
 */

module;

#include "FreeRTOS.h"
#include "semphr.h"

#include "emdevif/fault_handler.hpp"

export module emdevif.mutex:implements;
import :interface;

import emdevif.error_handler;

export namespace emdevif {

Mutex::StronglyTypedHandle Mutex::create(const Attribute& attribute)
{
    QueueHandle_t handle = nullptr;

    if (attribute.cb_mem != nullptr && attribute.cb_size != 0U) {
        handle = xSemaphoreCreateMutexStatic(static_cast<StaticQueue_t*>(attribute.cb_mem));
    }
    else {
        handle = xSemaphoreCreateMutex();
    }

    return {.value = handle};
}

void Mutex::destroy(Mutex& obj)  // NOLINT
{
    if (obj.handle_ != nullptr) {
        vSemaphoreDelete(obj.handle_);
        obj.handle_ = nullptr;
    }
}

inline ErrorCode Mutex::lock(const uint32_t timeout_tick) const
{
    const auto ret = xSemaphoreTake(static_cast<SemaphoreHandle_t>(handle_), timeout_tick);

    if (ret == pdTRUE) {
        return ErrorCode::Success;
    }
    else {
        return ErrorCode::Timeout;
    }
}

inline ErrorCode Mutex::try_lock() const
{
    return lock(0U);
}

inline void Mutex::unlock() const
{
    if (xSemaphoreGive(handle_) != pdTRUE) {
        EMDEVIF_FAULT_HANDLER("Failed to Unlock mutex!");
    }
}

Mutex::~Mutex()
{
    if (handle_ != nullptr) {
        this->destroy();
    }
}

}  // namespace emdevif
