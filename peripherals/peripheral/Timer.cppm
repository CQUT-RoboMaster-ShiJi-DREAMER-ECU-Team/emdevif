/**
 * @file Timer.cppm
 * @module emdevif.peripheral.timer
 * @author DuYicheng
 * @date 2025-10-04
 * @brief 定时器
 */

module;

#include <cstdint>

#include <string_view>

#include "emdevif/fatal_handler.h"

export module emdevif.peripheral.timer;

import emdevif.peripheralHandleMap;
import emdevif.errorHandler;
import emdevif.peripheral.base;
import emdevif.peripheral.model.timer;

export namespace emdevif {

class Timer : TimerModel
{
private:
    TimerModel::Instance* instance_;

public:
    explicit constexpr Timer(const std::string_view name) noexcept
        : instance_(static_cast<TimerModel::Instance*>(PeripheralHandleMap::findHandle(name).value_or(nullptr)))
    {
        internal::PeripheralErrorHandler::checkInstanceIsExist(instance_, "Timer");
    }

    void enable() const noexcept
    {
        EMDEVIF_ASSERT(instance_->enable != nullptr);
        instance_->enable(instance_->handle_);
    }

    void disable() const noexcept
    {
        EMDEVIF_ASSERT(instance_->disable != nullptr);
        instance_->disable(instance_->handle_);
    }

    [[nodiscard]] uint32_t getCounter() const noexcept
    {
        EMDEVIF_ASSERT(instance_->getCounter != nullptr);
        return instance_->getCounter(instance_->handle_);
    }

    void setCounter(const uint32_t counter) const noexcept
    {
        EMDEVIF_ASSERT(instance_->setCounter != nullptr);
        instance_->setCounter(instance_->handle_, counter);
    }

    void enableInterrupt() const noexcept
    {
        EMDEVIF_ASSERT(instance_->enableInterrupt != nullptr);
        instance_->enableInterrupt(instance_->handle_);
    }
    void disableInterrupt() const noexcept
    {
        EMDEVIF_ASSERT(instance_->disableInterrupt != nullptr);
        instance_->disableInterrupt(instance_->handle_);
    }

    void periodElapsedCallback(void* handle) const noexcept
    {
        EMDEVIF_ASSERT(instance_->periodElapsed != nullptr);
        instance_->periodElapsed(handle);
    }
};

}  // namespace emdevif
