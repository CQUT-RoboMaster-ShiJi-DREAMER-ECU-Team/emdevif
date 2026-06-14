/**
 * @file fixed_string.cppm
 * @brief 编译期固定大小字符串模块接口
 */

module;

#include <compare>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <string_view>
#include <limits>

#include "emdevif/core/fatal_handler.h"

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.data_container.fixed_string;

import emdevif.core.error_handler;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/data_container/fixed_string.hpp"
