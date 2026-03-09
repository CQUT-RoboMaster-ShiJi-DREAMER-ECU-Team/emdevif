/**
 * @file Spi.cppm
 * @brief SPI 通信
 */

module;

#include <cstdint>

#include <limits>
#include <span>
#include <string_view>
#include <type_traits>

#include "emdevif/core/fatal_handler.h"

#include "emdevif/core/simplify_decl_macros.hpp"

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.peripheral.spi;

import emdevif.core.error_handler;
import emdevif.peripheral.peripheral_handle_map;
import emdevif.peripheral.model.spi;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/peripheral/spi.hpp"
