/**
 * @file Gpio.cppm
 * @brief GPIO
 */

module;

#include <cstdint>
#include <string_view>

#include "emdevif/core/fatal_handler.h"

export module emdevif.peripheral.gpio;

import emdevif.core.error_handler;
import emdevif.peripheralHandleMap;
import emdevif.peripheral.base;
import emdevif.peripheral.model.gpio;

export namespace emdevif {

class Gpio : public GpioModel
{
private:
    GpioModel::Instance* const instance_;

public:
    explicit constexpr Gpio(const std::string_view name) noexcept
        : instance_(static_cast<GpioModel::Instance*>(PeripheralHandleMap::findHandle(name)))
    {
        detail::PeripheralErrorHandler::checkInstanceIsExist(instance_, "GPIO", name);
    }

    void write(const PinState pin_state) const noexcept
    {
        EMDEVIF_ASSERT(instance_->write_function != nullptr);
        instance_->write_function(instance_->handle, static_cast<uint_fast8_t>(pin_state));
    }

    [[nodiscard]] PinState read() const noexcept
    {
        EMDEVIF_ASSERT(instance_->write_function != nullptr);
        return (instance_->read_function(instance_->handle) == 0U ? Gpio::Reset : Gpio::Set);
    }

    void toggle() const noexcept
    {
        EMDEVIF_ASSERT(instance_->write_function != nullptr);
        instance_->toggle_function(instance_->handle);
    }
};

}  // namespace emdevif
