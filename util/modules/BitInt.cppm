/**
 * @file BitInt.cppm
 * @brief 指定宽度整型
 */

module;

#include <cstdint>

#include <type_traits>
#include <concepts>
#include <bit>
#include <numeric>
#include <iostream>
#include <compare>

#include "emdevif/util/bit_int/detail/exception_config.hpp"

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.util.bit_int;

import emdevif.core.concepts;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/util/bit_int.hpp"
