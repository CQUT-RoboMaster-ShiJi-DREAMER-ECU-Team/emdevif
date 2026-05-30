/**
 * @file try_finally.cppm
 * @brief try-finally 模式
 */

module;

#include <utility>
#include <concepts>
#include <functional>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.resource_guard.try_finally;

import emdevif.core.resource_guard.defer;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/resource_guard/try_finally.hpp"
