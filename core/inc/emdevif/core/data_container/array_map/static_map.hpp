/**
 * @file static_map.hpp
 * @brief 编译期静态键值对
 */

#pragma once
#ifndef EMDEVIF_CORE_DATA_CONTAINER_ARRAY_MAP_STATIC_MAP_HPP
#define EMDEVIF_CORE_DATA_CONTAINER_ARRAY_MAP_STATIC_MAP_HPP

#include <algorithm>
#include <array>
#include <iterator>
#include <utility>

#include "emdevif/core/data_container/array_map/basic_array_map.hpp"
#include "emdevif/core/fatal_handler.h"

#include "emdevif/core/error_handler.hpp"

namespace emdevif {

/**
 * 静态键值对
 * @tparam KeyType 键的类型
 * @tparam ValueType 值的类型
 * @tparam max_size 键值对最大数量
 */
template<typename KeyType, typename ValueType, std::size_t max_size, typename KeyEqual = std::equal_to<KeyType>>
class StaticMap : public BasicArrayMap<KeyType, ValueType, max_size, KeyEqual>
{
private:
    using SuperType_ = BasicArrayMap<KeyType, ValueType, max_size, KeyEqual>;

public:
    using key_type = SuperType_::key_type;
    using mapped_type = SuperType_::mapped_type;
    using value_type = SuperType_::value_type;
    using size_type = SuperType_::size_type;
    using difference_type = SuperType_::difference_type;
    using reference = SuperType_::reference;
    using const_reference = SuperType_::const_reference;
    using iterator = SuperType_::iterator;
    using const_iterator = SuperType_::const_iterator;
    using reverse_iterator = SuperType_::reverse_iterator;
    using const_reverse_iterator = SuperType_::const_reverse_iterator;

    consteval StaticMap() noexcept = default;
    consteval StaticMap(const StaticMap&) noexcept = default;
    consteval StaticMap(StaticMap&&) noexcept = default;

    ~StaticMap() noexcept = default;

    /**
     * 通过 std::array 类构造，用法与 @ref emdevif::ArrayMap 的构造函数相同
     */
    explicit consteval StaticMap(const std::array<value_type, max_size>& init_arr) noexcept : SuperType_(init_arr)
    {
        EMDEVIF_ASSERT(this->assertNoneRepeatKeys_(begin(), end()), "Have repeated keys!");
    }
    /**
     * @overload
     */
    explicit consteval StaticMap(const value_type (&init_arr)[max_size]) noexcept : SuperType_(init_arr)
    {
        EMDEVIF_ASSERT(this->assertNoneRepeatKeys_(begin(), end()), "Have repeated keys!");
    }

    [[nodiscard]] constexpr size_type size() const noexcept
    {
        return this->maxSize();
    }

    [[nodiscard]] constexpr size_type sizeByte() const noexcept
    {
        return this->maxSizeByte();
    }

    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return this->store_array_.begin();
    }
    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return this->store_array_.begin();
    }

    [[nodiscard]] constexpr iterator end() noexcept
    {
        return this->store_array_.end();
    }
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return this->store_array_.end();
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return this->store_array_.cbegin();
    }
    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return this->store_array_.cend();
    }

    /**
     * 查找特定的键
     * @param key 待查找的键
     * @return 查找到的键所在位置的迭代器。如果待查找的键不存在，返回 end()。
     */
    constexpr const_iterator find(const KeyType& key) const noexcept
    {
        return this->findInRange_(key, cbegin(), cend());
    }

    /**
     * 获取给定键的映射值
     * @attention 无边界检查。如果键不存在，属于未定义行为。
     * @param key 待获取映射后的值的键
     * @return 给定键的对应值
     */
    [[nodiscard]] constexpr mapped_type& operator[](const key_type& key) noexcept
    {
        return this->getItemInRange_(key, begin(), end());
    }
    /**
     * @overload
     */
    [[nodiscard]] constexpr const mapped_type& operator[](const key_type& key) const noexcept
    {
        return this->getItemInRange_(key, cbegin(), cend());
    }

    /**
     * 获取给定键的映射值的指针
     * @param key 待获取映射后的值的键
     * @return 给定键的对应值的指针。如果键不存在，返回 nullptr
     */
    [[nodiscard]] constexpr mapped_type* at(const key_type& key) noexcept
    {
        return this->atInRange_(key, begin(), end());
    }
    /**
     * @overload
     */
    [[nodiscard]] constexpr const mapped_type* at(const key_type& key) const noexcept
    {
        return this->atInRange_(key, cbegin(), cend());
    }
};

/**
 * 构造 @ref emdevif::StaticMap 类
 * @tparam KeyType 键的类型
 * @tparam ValueType 值的类型
 * @tparam size 构造的 StaticMap 类的键值对数量
 * @param init_arr 初始化所用的 C 风格数组
 * @return 构造好的 StaticMap 类
 */
template<typename KeyType, typename ValueType, std::size_t size, typename KeyEqual = std::equal_to<KeyType>>
consteval auto makeStaticMap(const std::pair<KeyType, ValueType> (&init_arr)[size]) noexcept
{
    return StaticMap<KeyType, ValueType, size, KeyEqual>{init_arr};
}

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_DATA_CONTAINER_ARRAY_MAP_STATIC_MAP_HPP
