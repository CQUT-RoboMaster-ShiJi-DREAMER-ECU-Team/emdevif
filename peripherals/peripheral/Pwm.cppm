/**
 * @file Pwm.cppm
 * @author DuYicheng
 * @date 2025-10-04
 * @brief PWM
 */

module;

#include <cstdint>
#include <string_view>

export module emdevif.peripheral.pwm;

import emdevif.peripheralHandleMap;
import emdevif.peripheral.base;
import emdevif.peripheral.model.pwm;

export namespace emdevif {

class Pwm : public PwmModel
{
private:
    PwmModel::Instance* instance_;

public:
    explicit constexpr Pwm(const std::string_view name) noexcept
        : instance_(static_cast<PwmModel::Instance*>(PeripheralHandleMap::findHandle(name).value_or(nullptr)))
    {
        internal::PeripheralErrorHandler::checkInstanceIsExist(instance_, "PWM");
    }

    void enable() const noexcept
    {
        instance_->enable_(instance_->handle_);
    }

    void disable() const noexcept
    {
        instance_->disable_(instance_->handle_);
    }

    void setRatio(const uint8_t ratio) const noexcept
    {
        instance_->setRatio_(instance_->handle_, ratio);
    }
};

}  // namespace emdevif
