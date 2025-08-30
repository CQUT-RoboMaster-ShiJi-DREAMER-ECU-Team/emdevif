/**
 * @file heap_memory_interface.cppm
 * @module emdevif.sys.heap:interface
 * @author DuYicheng
 * @date 2025-08-30
 * @brief 堆内存接口
 */

module;

#include <new>

export module emdevif.sys.heap:interface;

export namespace emdevif::heap {

template<typename T, typename... Args>
T* construct(Args&&... args);

template<typename T, typename... Args>
T* construct(std::nothrow_t, Args&&... args) noexcept;

template<typename T>
void destruct(T*& p) noexcept;

}  // namespace emdevif::heap
