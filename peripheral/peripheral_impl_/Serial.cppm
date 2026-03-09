/**
 * @file Serial.cppm
 * @brief 串口通信
 */

module;

#include <cstdint>

#include <limits>
#include <string_view>
#include <span>
#include <type_traits>

#include "emdevif/core/fatal_handler.h"

#include "emdevif/core/simplify_decl_macros.hpp"

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.peripheral.serial;

import emdevif.core.error_handler;
import emdevif.peripheral.peripheral_handle_map;
import emdevif.peripheral.model.serial;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/peripheral/serial.hpp"
