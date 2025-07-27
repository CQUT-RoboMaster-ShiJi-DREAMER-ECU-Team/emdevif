/**
 * @file ArrayTraitsAggregate.cppm
 * @module rmdev.util.ArrayTraitsAggregate
 * @author DuYicheng
 * @date 2025-07-24
 * @brief 数组特征的聚合类型
 */

module;

#include <array>

export module rmdev.util.ArrayTraitsAggregate;

export namespace rmdev {

#ifndef RMDEV_UTIL_ARRAY_TRAITS_AGGREGATE_ADD_RESERVE_MEMBER
#define RMDEV_UTIL_ARRAY_TRAITS_AGGREGATE_ADD_RESERVE_MEMBER false
#endif

/**
 * 有数组特征的聚合类型
 * @tparam Type 数据类型
 * @tparam size 数组长度
 */
template<typename Type, std::size_t size>
class ArrayTraitsAggregate
{
#if (RMDEV_UTIL_ARRAY_TRAITS_AGGREGATE_ADD_RESERVE_MEMBER)
private:
    // 用于避免警告: parameter passing for argument of type 'xxx' when C++17 is enabled
    // changed to match C++14 in GCC 10.1
    [[maybe_unused]] Type reserve{};
#endif

public:
    constexpr Type& operator[](const std::size_t n) noexcept
    {
#if (RMDEV_UTIL_ARRAY_TRAITS_AGGREGATE_ADD_RESERVE_MEMBER)
        return *(reinterpret_cast<Type*>(this) + n + 1);
#else
        return *(reinterpret_cast<Type*>(this) + n);
#endif
    }

    constexpr const Type& operator[](const std::size_t n) const noexcept
    {
        return const_cast<ArrayTraitsAggregate*>(this)->operator[](n);
    }

    constexpr Type* at(const std::size_t n)
    {
        if (n >= size) {
            return nullptr;
        }

        return &this->operator[](n);
    }

    constexpr const Type* at(const std::size_t n) const
    {
        return const_cast<ArrayTraitsAggregate*>(this)->at(n);
    }
};

}  // namespace rmdev
