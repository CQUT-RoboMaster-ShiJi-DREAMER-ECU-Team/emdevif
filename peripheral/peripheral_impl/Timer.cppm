/**
 * @file Timer.cppm
 * @brief 定时器
 */

module;

#include <cstdint>

#include <string_view>

#include "emdevif/core/fatal_handler.h"

export module emdevif.peripheral.timer;

import emdevif.peripheralHandleMap;
import emdevif.core.error_handler;
import emdevif.peripheral.base;
import emdevif.peripheral.model.timer;

export namespace emdevif {

class Timer : TimerModel
{
private:
    TimerModel::Instance* instance_;

public:
    explicit constexpr Timer(const std::string_view name) noexcept
        : instance_(static_cast<TimerModel::Instance*>(PeripheralHandleMap::findHandle(name)))
    {
        detail::PeripheralErrorHandler::checkInstanceIsExist(instance_, "Timer", name);
    }

    void enable() const noexcept
    {
        EMDEVIF_ASSERT(instance_->enable != nullptr);
        instance_->enable(instance_->handle);
    }

    void disable() const noexcept
    {
        EMDEVIF_ASSERT(instance_->disable != nullptr);
        instance_->disable(instance_->handle);
    }

    [[nodiscard]] uint32_t getCounter() const noexcept
    {
        EMDEVIF_ASSERT(instance_->getCounter != nullptr);
        return instance_->getCounter(instance_->handle);
    }

    void setCounter(const uint32_t counter) const noexcept
    {
        EMDEVIF_ASSERT(instance_->setCounter != nullptr);
        instance_->setCounter(instance_->handle, counter);
    }

    void enableInterrupt() const noexcept
    {
        EMDEVIF_ASSERT(instance_->enableInterrupt != nullptr);
        instance_->enableInterrupt(instance_->handle);
    }
    void disableInterrupt() const noexcept
    {
        EMDEVIF_ASSERT(instance_->disableInterrupt != nullptr);
        instance_->disableInterrupt(instance_->handle);
    }

    void periodElapsedCallback(void* handle) const noexcept
    {
        EMDEVIF_ASSERT(instance_->periodElapsed != nullptr);
        instance_->periodElapsed(handle);
    }
};

}  // namespace emdevif
