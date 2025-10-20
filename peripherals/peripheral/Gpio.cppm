/**
 * @file Gpio.cppm
 * @author DuYicheng
 * @date 2025-10-04
 * @brief GPIO
 */

module;

#include <cstdint>
#include <string_view>

#include "emdevif/fatal_handler.h"

export module emdevif.peripheral.gpio;

import emdevif.errorHandler;
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
        : instance_(static_cast<GpioModel::Instance*>(PeripheralHandleMap::findHandle(name).value_or(nullptr)))
    {
        internal::PeripheralErrorHandler::checkInstanceIsExist(instance_, "GPIO");
    }

    void write(const PinState pin_state) const noexcept
    {
        EMDEVIF_ASSERT(instance_->write_function_ != nullptr);
        instance_->write_function_(instance_->handle_, static_cast<uint_fast8_t>(pin_state));
    }

    [[nodiscard]] PinState read() const noexcept
    {
        EMDEVIF_ASSERT(instance_->write_function_ != nullptr);
        return (instance_->read_function_(instance_->handle_) == 0U ? Gpio::Reset : Gpio::Set);
    }

    void toggle() const noexcept
    {
        EMDEVIF_ASSERT(instance_->write_function_ != nullptr);
        instance_->toggle_function_(instance_->handle_);
    }
};

}  // namespace emdevif
