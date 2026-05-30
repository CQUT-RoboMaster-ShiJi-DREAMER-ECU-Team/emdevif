/**
 * @file defer.cppm
 * @brief 延迟执行守卫
 */

module;

#include <utility>
#include <concepts>
#include <functional>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.resource_guard.defer;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/resource_guard/defer.hpp"
