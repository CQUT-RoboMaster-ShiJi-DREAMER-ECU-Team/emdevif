/**
 * @file gpio.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_MODEL_GPIO_HPP
    #define EMDEVIF_PERIPHERAL_MODEL_GPIO_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include <cstdint>

        #include <type_traits>
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

class GpioModel
{
public:
    enum PinState : uint_fast8_t {
        Reset = 0,
        Set = 1
    };

    using WriteFunction = void (*)(void* handle, uint_fast8_t pin_state);
    using ReadFunction = uint_fast8_t (*)(void* handle);
    using ToggleFunction = void (*)(void* handle);

    struct Instance {
        void* const handle{nullptr};

        const WriteFunction write_function{nullptr};
        const ReadFunction read_function{nullptr};
        const ToggleFunction toggle_function{nullptr};
    };
};

static_assert(std::is_standard_layout_v<GpioModel::Instance>);

}  // namespace emdevif

#endif  // !EMDEVIF_PERIPHERAL_MODEL_GPIO_HPP
