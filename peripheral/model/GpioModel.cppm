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
        void* const handle{nullptr};

        const WriteFunction write_function{nullptr};
        const ReadFunction read_function{nullptr};
        const ToggleFunction toggle_function{nullptr};
    };
};

static_assert(std::is_standard_layout_v<GpioModel::Instance>);

}  // namespace emdevif
