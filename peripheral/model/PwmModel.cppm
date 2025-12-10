/**
 * @file PwmModel.cppm
 * @brief PWM 模型
 */

module;

#include <cstdint>

#include <type_traits>

export module emdevif.peripheral.model.pwm;

export namespace emdevif {

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
