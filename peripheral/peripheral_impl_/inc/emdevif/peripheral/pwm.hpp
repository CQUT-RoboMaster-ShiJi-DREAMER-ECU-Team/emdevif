/**
 * @file pwm.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_PWM_HPP
    #define EMDEVIF_PERIPHERAL_PWM_HPP

    #include "emdevif/core/detail/config.hpp"
    #include "emdevif/peripheral/detail/peripheral_error_handler.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/peripheral/peripheral_handle_map.hpp"
        #include "emdevif/peripheral/model/pwm.hpp"
        #include "emdevif/core/error_handler.hpp"

        #include <cstdint>

        #include <span>
        #include <string_view>

        #include "emdevif/core/fatal_handler.h"
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

class Pwm : public PwmModel
{
private:
    PwmModel::Instance* instance_;

public:
    explicit constexpr Pwm(const std::string_view name) noexcept
        : instance_(static_cast<PwmModel::Instance*>(PeripheralHandleMap::findHandle(name)))
    {
        detail::PeripheralErrorHandler::checkInstanceIsExist(instance_, "PWM", name);
    }

    void enable() const noexcept
    {
        instance_->enable(instance_->handle);
    }

    void disable() const noexcept
    {
        instance_->disable(instance_->handle);
    }

    void setRatio(const uint8_t ratio) const noexcept
    {
        instance_->setRatio(instance_->handle, ratio);
    }
};

}  // namespace emdevif

#endif  // !EMDEVIF_PERIPHERAL_PWM_HPP
