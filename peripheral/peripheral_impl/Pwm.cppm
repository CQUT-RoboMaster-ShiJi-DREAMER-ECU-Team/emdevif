/**
 * @file Pwm.cppm
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
