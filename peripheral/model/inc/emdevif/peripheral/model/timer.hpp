/**
 * @file timer.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_MODEL_TIMER_HPP
    #define EMDEVIF_PERIPHERAL_MODEL_TIMER_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include <cstdint>

        #include <span>
        #include <type_traits>
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

class TimerModel
{
public:
    using EnableFunction = void (*)(void* handle);
    using DisableFunction = void (*)(void* handle);

    using GetCounterFunction = uint32_t (*)(void* handle);
    using SetCounterFunction = void (*)(void* handle, uint32_t counter);

    using EnableInterruptFunction = void (*)(void* handle);
    using DisableInterruptFunction = void (*)(void* handle);

    using PeriodElapsedCallback = void (*)(void* handle);

    struct Instance {
        void* const handle{nullptr};

        const EnableFunction enable{nullptr};
        const DisableFunction disable{nullptr};

        const GetCounterFunction getCounter{nullptr};
        const SetCounterFunction setCounter{nullptr};

        const EnableInterruptFunction enableInterrupt{nullptr};
        const DisableInterruptFunction disableInterrupt{nullptr};

        const PeriodElapsedCallback periodElapsed{nullptr};
    };
};

static_assert(std::is_standard_layout_v<TimerModel::Instance>);

}  // namespace emdevif

#endif  // !EMDEVIF_PERIPHERAL_MODEL_TIMER_HPP
