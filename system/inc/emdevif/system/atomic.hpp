/**
 * @file atomic.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_SYSTEM_ATOMIC_HPP
#define EMDEVIF_SYSTEM_ATOMIC_HPP

#include "emdevif/core/detail/config.hpp"

#ifndef EMDEVIF_USE_STL_ATOMIC
#define EMDEVIF_USE_STL_ATOMIC true
#endif

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#if (EMDEVIF_USE_STL_ATOMIC)
#include <atomic>
#endif
#endif

#if (EMDEVIF_USE_STL_ATOMIC)

EMDEVIF_MODULE_EXPORT
namespace emdevif {

template<typename T>
using atomic = std::atomic<T>;

using memory_order = std::memory_order;

}  // namespace emdevif

#endif  // EMDEVIF_USE_STL_ATOMIC

#endif  // !EMDEVIF_SYSTEM_ATOMIC_HPP
