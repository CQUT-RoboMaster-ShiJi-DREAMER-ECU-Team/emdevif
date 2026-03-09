/**
 * @file PeripheralHandleMap.cppm
 * @brief
 */

module;

#include <string_view>
#include <type_traits>

#include "emdevif/core/simplify_decl_macros.hpp"

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.peripheral.peripheral_handle_map;

import emdevif.core.type_traits;
import emdevif.userDeclares;
import emdevif.core.data_container.array_map;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/peripheral/peripheral_handle_map.hpp"
