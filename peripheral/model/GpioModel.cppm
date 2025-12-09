/**
 * @file GpioModel.cppm
 * @brief GPIO 模型
 */

module;

#include <cstdint>

#include <type_traits>

export module emdevif.peripheral.model.gpio;

export namespace emdevif {

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
        void* const handle_{nullptr};

        const WriteFunction write_function_{noWrite};
        const ReadFunction read_function_{noRead};
        const ToggleFunction toggle_function_{nullptr};
    };

private:
    static void noWrite(void*, uint_fast8_t) noexcept;
    static uint_fast8_t noRead(void*) noexcept;
};

static_assert(std::is_standard_layout_v<GpioModel::Instance>);

}  // namespace emdevif

// module :private;

namespace emdevif {

void GpioModel::noWrite(void*, uint_fast8_t) noexcept {}

uint_fast8_t GpioModel::noRead(void*) noexcept
{
    return 0;
}

}  // namespace emdevif
