/**
 * @file Gpio.cppm
 * @module emdevif.peripheralModels.gpio
 * @author DuYicheng
 * @date 2025-10-04
 * @brief GPIO
 */

module;

#include <cstdint>
#include <string_view>

export module emdevif.peripheralModels.gpio;

import emdevif.errorHandler;
import emdevif.peripheralHandleMap;
import emdevif.peripheralModels.errorHandler;

export namespace emdevif {

class Gpio
{
public:
    enum PinState : uint_fast8_t {
        Reset = 0,
        Set = 1
    };

    using WriteFunction = void (&)(void* handle, uint_fast8_t pin_state);
    using ReadFunction = uint_fast8_t (&)(void* handle);
    using ToggleFunction = void (*)(void* handle);

private:
    void* handle_;

    WriteFunction write_function_;
    ReadFunction read_function_;
    ToggleFunction toggle_function_;

public:
    explicit constexpr Gpio(const std::string_view name,
                            WriteFunction write_function = noWrite,
                            ReadFunction read_function = noRead,
                            const ToggleFunction toggle_function = nullptr) noexcept
        : handle_(PeripheralHandleMap::findHandle(name).value_or(nullptr)),
          write_function_(write_function),
          read_function_(read_function),
          toggle_function_(toggle_function)
    {
        internal::checkHandleIsExist(handle_, "GPIO");
    }

    void write(const PinState pin_state) const noexcept
    {
        write_function_(handle_, static_cast<uint_fast8_t>(pin_state));
    }

    [[nodiscard]] PinState read() const noexcept
    {
        return (read_function_(handle_) == 0U ? Reset : Set);
    }

    void toggle() const noexcept
    {
        if (toggle_function_ != nullptr) {
            toggle_function_(handle_);
        }
    }

private:
    static void noWrite(void*, uint_fast8_t) noexcept;
    static uint_fast8_t noRead(void*) noexcept;
};

}  // namespace emdevif

// module :private;

namespace emdevif {

void Gpio::noWrite(void*, uint_fast8_t) noexcept {}

uint_fast8_t Gpio::noRead(void*) noexcept
{
    return 0;
}

}  // namespace emdevif
