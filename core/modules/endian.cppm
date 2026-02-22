/**
 * @file endian.cppm
 * @brief 端序
 */

module;

#include <cstdint>

#include <concepts>
#include <bit>
#include <array>
#include <algorithm>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.endian;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/endian.hpp"
