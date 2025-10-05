/**
 * @file Pwm.cppm
 * @author DuYicheng
 * @date 2025-10-04
 * @brief PWM
 */

module;

#include <cstdint>
#include <string_view>

export module emdevif.peripheralModels.pwm;

import emdevif.peripheralHandleMap;
import emdevif.peripheralModels.errorHandler;

export namespace emdevif {

class Pwm
{
public:
    using EnableFunction = void (&)(void* handle);
    using DisableFunction = void (&)(void* handle);

    using setRatioFunction = void (&)(void* handle, uint8_t ratio);

private:
    void* handle_;

    EnableFunction enable_;
    DisableFunction disable_;
    setRatioFunction setRatio_;

public:
    constexpr Pwm(const std::string_view name,
                  EnableFunction enable,
                  DisableFunction disable,
                  setRatioFunction setRatio) noexcept
        : handle_(PeripheralHandleMap::findHandle(name).value_or(nullptr)),
          enable_(enable),
          disable_(disable),
          setRatio_(setRatio)
    {
        internal::checkHandleIsExist(handle_, "PWM");
    }

    void enable() const noexcept
    {
        enable_(handle_);
    }

    void disable() const noexcept
    {
        disable_(handle_);
    }

    void setRatio(const uint8_t ratio) const noexcept
    {
        setRatio_(handle_, ratio);
    }
};

}  // namespace emdevif
