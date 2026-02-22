/**
 * @file type_traits.cppm
 * @brief 类型特征（STL type_traits 头文件的扩展）
 */

module;

#include <type_traits>
#include <utility>
#include <tuple>
#include <array>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.type_traits;

import boost.pfr;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/type_traits.hpp"
