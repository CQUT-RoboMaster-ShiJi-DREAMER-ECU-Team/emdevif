/**
 * @file Can.cppm
 * @brief CAN 通信
 */

module;

#include <cstdint>

#include <span>
#include <string_view>
#include <type_traits>

#include "emdevif/core/fatal_handler.h"

#include "emdevif/core/simplify_decl_macros.hpp"

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.peripheral.can;

import emdevif.core.error_handler;
import emdevif.peripheral.peripheral_handle_map;
import emdevif.peripheral.model.can;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/peripheral/can.hpp"
