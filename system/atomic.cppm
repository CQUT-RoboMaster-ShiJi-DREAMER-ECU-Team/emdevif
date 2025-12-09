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

export module emdevif.sys.atomic;

export namespace emdevif {

#if (EMDEVIF_USE_STL_ATOMIC)

template<typename T>
using atomic = std::atomic<T>;

using memory_order = std::memory_order;

#endif

}  // namespace emdevif
