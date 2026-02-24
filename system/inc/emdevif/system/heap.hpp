/**
 * @file heap.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_SYSTEM_HEAP_HPP
#define EMDEVIF_SYSTEM_HEAP_HPP

#include "emdevif/core/detail/config.hpp"

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#include "emdevif/core/concepts.hpp"

#include <memory>
#include <utility>
#include <type_traits>
#endif

EMDEVIF_MODULE_EXPORT
namespace emdevif::heap {

template<typename T, typename... Args>
T* construct(Args&&... args);

template<typename T, typename... Args>
T* construct(std::nothrow_t, Args&&... args) noexcept;

template<PointerType T>
void destruct(T& p) noexcept;

template<typename T>
class Deleter;

template<typename T>
using unique_ptr = std::unique_ptr<T, Deleter<T>>;

template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args);

template<typename T, typename... Args>
unique_ptr<T> make_unique(std::nothrow_t, Args&&... args) noexcept;

// 使用时除了导入模块，还需要引入头文件 <memory>
template<typename T>
class Allocator;

}  // namespace emdevif::heap

#include "emdevif/system_impl/heap.inl"

#endif  // !EMDEVIF_SYSTEM_HEAP_HPP
