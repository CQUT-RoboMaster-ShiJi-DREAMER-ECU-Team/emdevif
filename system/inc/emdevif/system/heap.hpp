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

/**
 * @brief 在系统堆上构造对象
 * @tparam T 对象类型
 * @tparam Args 构造参数类型包
 * @param args 构造参数
 * @return 指向已构造对象的指针。若失败，调用 EMDEVIF_FATAL_HANDLER
 */
template<typename T, typename... Args>
T* construct(Args&&... args);

/**
 * @brief 在系统堆上构造对象（不抛出版本）
 * @tparam T 对象类型
 * @tparam Args 构造参数类型包
 * @param args 构造参数
 * @return 指向已构造对象的指针，失败返回 nullptr
 */
template<typename T, typename... Args>
T* construct(std::nothrow_t, Args&&... args) noexcept;

/**
 * @brief 销毁系统堆上的对象并释放内存
 * @tparam T 指针类型，需满足 PointerType 概念
 * @param p 待销毁的指针引用（解引用后置为 nullptr）
 */
template<PointerType T>
void destruct(T& p) noexcept;

/**
 * @brief 堆分配对象的删除器
 * @tparam T 对象类型
 */
template<typename T>
class Deleter;

/**
 * @brief unique_ptr 类型别名，使用 heap::Deleter 作为删除器
 * @tparam T 对象类型
 */
template<typename T>
using unique_ptr = std::unique_ptr<T, Deleter<T>>;

/**
 * @brief 在系统堆上构造对象并返回 unique_ptr
 * @tparam T 对象类型
 * @tparam Args 构造参数类型包
 * @param args 构造参数
 * @return 管理对象的 unique_ptr
 */
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args);

/**
 * @brief 在系统堆上构造对象并返回 unique_ptr（不抛出版本）
 * @tparam T 对象类型
 * @tparam Args 构造参数类型包
 * @param args 构造参数
 * @return 管理对象的 unique_ptr，失败返回 nullptr
 */
template<typename T, typename... Args>
unique_ptr<T> make_unique(std::nothrow_t, Args&&... args) noexcept;

/**
 * @brief 系统堆分配器，与 STL 容器兼容
 * @attention 使用这个类时除了要导入模块，还需要引入头文件 <memory>，否则可能提示“找不到 std::allocator_trailts 的定义”
 * @tparam T 分配的元素类型
 */
template<typename T>
class Allocator;

}  // namespace emdevif::heap

#include "emdevif/system_impl/heap.inl"

#endif  // !EMDEVIF_SYSTEM_HEAP_HPP
