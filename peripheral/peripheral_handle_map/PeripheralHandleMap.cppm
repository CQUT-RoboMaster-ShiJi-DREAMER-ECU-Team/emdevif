/**
 * @file PeripheralHandleMap.cppm
 * @brief 外设句柄映射 C++20 模块封装，重导出 peripheral_handle_map.hpp 中的接口
 */

module;

#include <string_view>
#include <type_traits>

#include "emdevif/core/simplify_decl_macros.hpp"

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.peripheral.peripheral_handle_map;

import emdevif.core.type_traits;
import emdevif.user_declares;
import emdevif.core.data_container.array_map;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/peripheral/peripheral_handle_map.hpp"
