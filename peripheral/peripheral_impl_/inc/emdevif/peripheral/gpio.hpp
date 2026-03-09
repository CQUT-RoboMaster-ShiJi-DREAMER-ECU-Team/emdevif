/**
 * @file gpio.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_GPIO_HPP
    #define EMDEVIF_PERIPHERAL_GPIO_HPP

    #include "emdevif/core/detail/config.hpp"
    #include "emdevif/peripheral/detail/peripheral_error_handler.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/peripheral/peripheral_handle_map.hpp"
        #include "emdevif/peripheral/model/gpio.hpp"
        #include "emdevif/core/error_handler.hpp"

        #include <cstdint>

        #include <span>
        #include <string_view>

        #include "emdevif/core/fatal_handler.h"
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

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

#endif  // !EMDEVIF_PERIPHERAL_GPIO_HPP
