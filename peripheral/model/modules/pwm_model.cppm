/**
 * @file pwm_model.cppm
 * @brief PWM 模型
 */

module;

#include <cstdint>

#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.peripheral.model.pwm;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/peripheral/model/pwm.hpp"
