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
        void* const handle_;

        const EnableFunction enable_;
        const DisableFunction disable_;
        const setRatioFunction setRatio_;
    };
};

static_assert(std::is_standard_layout_v<PwmModel::Instance>);

}  // namespace emdevif
