/**
 * @file init_guard.cppm
 * @brief RAII 风格的 init/deInit 自动管理
 */

module;

#include <utility>
#include <concepts>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.resource_guard.init_guard;

import emdevif.core.error_handler;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/resource_guard/init_guard.hpp"
