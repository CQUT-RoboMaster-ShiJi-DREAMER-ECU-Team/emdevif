/**
 * @file lock_guard.cppm
 * @brief 互斥锁封装器
 */

module;

#include <utility>
#include <concepts>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.lock_guard;

import emdevif.core.error_handler;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/lock_guard.hpp"
