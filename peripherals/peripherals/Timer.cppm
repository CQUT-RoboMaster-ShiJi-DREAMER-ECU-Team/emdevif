/**
 * @file Timer.cppm
 * @author DuYicheng
 * @date 2025-10-04
 * @brief 定时器
 */

module;

#include <cstdint>

#include <string_view>

#include "emdevif/fatal_handler.hpp"

export module emdevif.peripheralModels.timer;

import emdevif.peripheralHandleMap;
import emdevif.errorHandler;
import emdevif.peripheralModels.errorHandler;

export namespace emdevif {

class Timer
{
public:
    using EnableFunction = void (&)(void* handle);
    using DisableFunction = void (&)(void* handle);

    using GetCounterFunction = uint32_t (&)(void* handle);
    using SetCounterFunction = void (&)(void* handle, uint32_t counter);

    using EnableInterruptFunction = void (&)(void* handle);
    using DisableInterruptFunction = void (&)(void* handle);

    using PeriodElapsedCallback = void (*)(void* handle);

    struct BehaviourFunctions {
        EnableFunction enable;
        DisableFunction disable;

        GetCounterFunction getCounter;
        SetCounterFunction setCounter;

        EnableInterruptFunction enableInterrupt;
        DisableInterruptFunction disableInterrupt;

        PeriodElapsedCallback periodElapsed{nullptr};
    };

private:
    void* handle_;

    BehaviourFunctions behaviour_;

public:
    constexpr Timer(const std::string_view name, const BehaviourFunctions& behaviour) noexcept
        : handle_(PeripheralHandleMap::findHandle(name).value_or(nullptr)), behaviour_(behaviour)
    {
        internal::checkHandleIsExist(handle_, "Timer");
    }

    void enable() const noexcept
    {
        behaviour_.enable(handle_);
    }

    void disable() const noexcept
    {
        behaviour_.disable(handle_);
    }

    [[nodiscard]] uint32_t getCounter() const noexcept
    {
        return behaviour_.getCounter(handle_);
    }

    void setCounter(const uint32_t counter) const noexcept
    {
        behaviour_.setCounter(handle_, counter);
    }

    void enableInterrupt() const noexcept
    {
        behaviour_.enableInterrupt(handle_);
    }
    void disableInterrupt() const noexcept
    {
        behaviour_.disableInterrupt(handle_);
    }

    void periodElapsedCallback(void* handle) const noexcept
    {
        if (behaviour_.periodElapsed != nullptr) {
            behaviour_.periodElapsed(handle);
        }
    }
};

}  // namespace emdevif
