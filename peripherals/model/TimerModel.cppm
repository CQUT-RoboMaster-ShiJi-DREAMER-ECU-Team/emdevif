/**
 * @file TimerModel.cppm
 * @author DuYicheng
 * @date 2025-10-13
 * @brief 定时器模型
 */

module;

#include <cstdint>

export module emdevif.peripheral.model.timer;

export namespace emdevif {

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
        void* const handle_;

        const EnableFunction enable;
        const DisableFunction disable;

        const GetCounterFunction getCounter;
        const SetCounterFunction setCounter;

        const EnableInterruptFunction enableInterrupt;
        const DisableInterruptFunction disableInterrupt;

        const PeriodElapsedCallback periodElapsed{nullptr};
    };
};

}  // namespace emdevif
