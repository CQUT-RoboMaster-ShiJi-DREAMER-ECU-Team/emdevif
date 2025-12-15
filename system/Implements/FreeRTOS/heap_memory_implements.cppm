/**
 * @file heap_memory_implements.cppm
 * @brief 堆内存 - FreeRTOS 实现
 */

module;

#include <memory>
#include <utility>
#include <type_traits>

#if (defined(EMDEVIF_THREAD_USE_ESPIDF_FREERTOS) && EMDEVIF_THREAD_USE_ESPIDF_FREERTOS)
#include "freertos/FreeRTOS.h"
#else
#include "FreeRTOS.h"
#endif

export module emdevif.sys.heap:implements;

import emdevif.concepts;

namespace emdevif::heap::internal {

void* mallocByte(const std::size_t size_in_bytes) noexcept
{
    return pvPortMalloc(size_in_bytes);
}

void free(void* block) noexcept
{
    vPortFree(block);
}

}  // namespace emdevif::heap::internal

export namespace emdevif::heap {

template<typename T, typename... Args>
T* construct(Args&&... args)
{
    static_assert(!std::is_void_v<T>, "can't create pointer to incomplete type");
    static_assert(sizeof(T) > 0, "can't create pointer to incomplete type");

    auto ret = static_cast<T*>(internal::mallocByte(sizeof(T)));
    if (ret == nullptr) {
        throw std::bad_alloc();
    }

    try {
        std::construct_at(ret, std::forward<Args>(args)...);
    }
    catch (...) {
        internal::free(ret);
        throw;
    }

    return ret;
}

template<typename T, typename... Args>
T* construct(std::nothrow_t, Args&&... args) noexcept
{
    static_assert(!std::is_void_v<T>, "can't create pointer to incomplete type");
    static_assert(sizeof(T) > 0, "can't create pointer to incomplete type");

    auto ret = static_cast<T*>(internal::mallocByte(sizeof(T)));
    if (ret == nullptr) {
        return nullptr;
    }

    std::construct_at(ret, std::forward<Args>(args)...);

    return ret;
}

template<PointerType T>
void destruct(T& p) noexcept
{
    static_assert(!std::is_void_v<T> && !std::is_void_v<std::remove_pointer_t<T>>,
                  "can't delete pointer to incomplete type");
    static_assert(sizeof(T) > 0, "can't delete pointer to incomplete type");

    if (p == nullptr) {
        return;
    }

    std::destroy_at(p);

    internal::free(p);

    p = nullptr;
}

template<typename T>
class Deleter
{
public:
    void operator()(T* p) const
    {
        static_assert(!std::is_void_v<T>, "can't delete pointer to incomplete type");
        static_assert(sizeof(T) > 0, "can't delete pointer to incomplete type");

        destruct(p);
    }
};

template<typename T>
using unique_ptr = std::unique_ptr<T, Deleter<T>>;

template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
    try {
        return unique_ptr<T>{construct<T>(std::forward<Args>(args)...)};
    }
    catch (...) {
        throw;
    }
}

template<typename T, typename... Args>
unique_ptr<T> make_unique(std::nothrow_t, Args&&... args) noexcept
{
    return unique_ptr<T>{construct<T>(std::nothrow, std::forward<Args>(args)...)};
}

template<typename T>
class Allocator
{
public:
    using pointer = T*;
    using const_pointer = const T*;

    using void_pointer = void*;
    using const_void_pointer = const void*;

    using value_type = T;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

public:
    Allocator() noexcept = default;

    // ReSharper disable once CppNonExplicitConvertingConstructor
    template<typename U>
    Allocator(const Allocator<U>&) noexcept  // NOLINT(*-explicit-constructor)
    {
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    pointer allocate(const size_type n)
    {
        return static_cast<pointer>(internal::mallocByte(n * sizeof(value_type)));
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppParameterMayBeConst
    void deallocate(pointer p, const size_type)
    {
        internal::free(p);
    }
};

}  // namespace emdevif::heap
