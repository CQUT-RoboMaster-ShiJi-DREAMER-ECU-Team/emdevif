/**
 * @file timer.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_TIMER_HPP
    #define EMDEVIF_PERIPHERAL_TIMER_HPP

    #include "emdevif/core/detail/config.hpp"
    #include "emdevif/peripheral/detail/peripheral_error_handler.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/peripheral/peripheral_handle_map.hpp"
        #include "emdevif/peripheral/model/timer.hpp"
        #include "emdevif/core/error_handler.hpp"

        #include <cstdint>

        #include <string_view>

        #include "emdevif/core/fatal_handler.h"
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

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

#endif  // !EMDEVIF_PERIPHERAL_TIMER_HPP
