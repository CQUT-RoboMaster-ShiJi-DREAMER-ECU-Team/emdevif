/**
 * @file Timer.cppm
 * @brief 定时器
 */

module;

#include <cstdint>

#include <string_view>
#include <type_traits>

#include "emdevif/core/fatal_handler.h"

#include "emdevif/core/simplify_decl_macros.hpp"

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.peripheral.timer;

import emdevif.peripheral.peripheral_handle_map;
import emdevif.core.error_handler;
import emdevif.peripheral.model.timer;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/peripheral/timer.hpp"
