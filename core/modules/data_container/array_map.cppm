/**
 * @file array_map.cppm
 * @brief 数组键值对（通过固定大小的数组实现的键值对容器）
 */

module;

#include "emdevif/core/fatal_handler.h"

#include <cstring>

#include <utility>
#include <type_traits>
#include <concepts>
#include <array>
#include <algorithm>
#include <iterator>
#include <functional>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.data_container.array_map;

import emdevif.core.error_handler;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/data_container/array_map.hpp"
