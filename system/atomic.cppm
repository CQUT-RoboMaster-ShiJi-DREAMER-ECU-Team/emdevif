/**
 * @file atomic.cppm
 * @brief 原子操作
 */

module;

#ifndef EMDEVIF_USE_STL_ATOMIC
#define EMDEVIF_USE_STL_ATOMIC true
#endif

#if (EMDEVIF_USE_STL_ATOMIC)
#include <atomic>
#endif

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.system.atomic;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/system/atomic.hpp"
