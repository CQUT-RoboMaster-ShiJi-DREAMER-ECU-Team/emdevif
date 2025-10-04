/**
 * @file Gpio.cppm
 * @module emdevif.peripheralModels.gpio
 * @author DuYicheng
 * @date 2025-10-04
 * @brief GPIO
 */

module;

#include <cstdint>

export module emdevif.peripheralModels.gpio;

export namespace emdevif {

class Gpio
{
public:
    enum PinState : uint_fast8_t {
        Reset = 0,
        Set = 1
    };

    using WriteFunction = void (&)(void* port, uint32_t pin, uint_fast8_t pin_state);
    using ReadFunction = uint_fast8_t (&)(void* port, uint32_t pin);
    using ToggleFunction = void (*)(void* port, uint32_t pin);

private:
    void* port_;
    uint32_t pin_;

    WriteFunction write_function_;
    ReadFunction read_function_;
    ToggleFunction toggle_function_;

public:
    constexpr Gpio(void* port,
                   const uint32_t pin,
                   WriteFunction write_function,
                   ReadFunction read_function,
                   const ToggleFunction toggle_function = nullptr) noexcept
        : port_(port),
          pin_(pin),
          write_function_(write_function),
          read_function_(read_function),
          toggle_function_(toggle_function)
    {
    }

    void write(const PinState pin_state) const noexcept
    {
        write_function_(port_, pin_, static_cast<uint_fast8_t>(pin_state));
    }

    [[nodiscard]] PinState read() const noexcept
    {
        return (read_function_(port_, pin_) == 0U ? Reset : Set);
    }

    void toggle() const noexcept
    {
        if (toggle_function_ != nullptr) {
            toggle_function_(port_, pin_);
        }
    }
};

}  // namespace emdevif
