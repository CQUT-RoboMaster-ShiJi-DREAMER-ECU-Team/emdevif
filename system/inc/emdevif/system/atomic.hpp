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

/**
 * @brief STL std::atomic 的类型别名
 * @tparam T 原子操作的类型
 */
template<typename T>
using atomic = std::atomic<T>;

/**
 * @brief STL std::memory_order 的类型别名，用于指定原子操作的内存序
 */
using memory_order = std::memory_order;

}  // namespace emdevif

#endif  // EMDEVIF_USE_STL_ATOMIC

#endif  // !EMDEVIF_SYSTEM_ATOMIC_HPP
