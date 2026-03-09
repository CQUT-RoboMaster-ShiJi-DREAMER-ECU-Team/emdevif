/**
 * @file Pwm.cppm
 * @brief PWM
 */

module;

#include <cstdint>
#include <string_view>
#include <type_traits>

#include "emdevif/core/fatal_handler.h"

#include "emdevif/core/simplify_decl_macros.hpp"

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.peripheral.pwm;

import emdevif.peripheral.peripheral_handle_map;
import emdevif.peripheral.model.pwm;
import emdevif.core.error_handler;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/peripheral/pwm.hpp"
