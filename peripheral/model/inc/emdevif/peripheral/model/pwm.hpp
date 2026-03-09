/**
 * @file pwm.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_MODEL_PWM_HPP
    #define EMDEVIF_PERIPHERAL_MODEL_PWM_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include <cstdint>

        #include <type_traits>
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

class PwmModel
{
public:
    using EnableFunction = void (*)(void* handle);
    using DisableFunction = void (*)(void* handle);

    using setRatioFunction = void (*)(void* handle, uint8_t ratio);

    struct Instance {
        void* const handle{nullptr};

        const EnableFunction enable{nullptr};
        const DisableFunction disable{nullptr};
        const setRatioFunction setRatio{nullptr};
    };
};

static_assert(std::is_standard_layout_v<PwmModel::Instance>);

}  // namespace emdevif

#endif  // !EMDEVIF_PERIPHERAL_MODEL_PWM_HPP
