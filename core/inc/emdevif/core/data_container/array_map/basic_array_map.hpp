/**
 * @file basic_array_map.hpp
 * @brief 数组键值对 - 内部基本功能
 */

#pragma once
#ifndef EMDEVIF_CORE_DATA_CONTAINER_ARRAY_MAP_BASIC_ARRAY_MAP_HPP
    #define EMDEVIF_CORE_DATA_CONTAINER_ARRAY_MAP_BASIC_ARRAY_MAP_HPP

        #include <algorithm>
        #include <array>
        #include <functional>
        #include <iterator>
        #include <utility>

        #include "emdevif/core/fatal_handler.h"

        #include "emdevif/core/error_handler.hpp"

namespace emdevif {

/**
 * 以数组存储的键值对 - 基本结构，不带记录头部的指针的功能
 * @tparam KeyType 键的类型
 * @tparam ValueType 值的类型
 * @tparam KeyEqual 用于比较键是否相等的函数类型
 * @tparam max_size 键值对最大数量
 */
template<typename KeyType, typename ValueType, std::size_t max_size, typename KeyEqual = std::equal_to<KeyType>>
class BasicArrayMap
{
public:
    static_assert(max_size > 0, "The max_size must be greater than 0");

    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<key_type, mapped_type>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = typename std::array<value_type, max_size>::iterator;
    using const_iterator = typename std::array<value_type, max_size>::const_iterator;
    using reverse_iterator = typename std::array<value_type, max_size>::reverse_iterator;
    using const_reverse_iterator = typename std::array<value_type, max_size>::const_reverse_iterator;

protected:
    std::array<value_type, max_size> store_array_;  ///< 内部存储所用的数组
    [[no_unique_address]] KeyEqual key_equal_{};    ///< 用于比较键是否相等的函数

public:
    constexpr BasicArrayMap() noexcept = default;
    constexpr BasicArrayMap(const BasicArrayMap&) noexcept = default;
    constexpr BasicArrayMap(BasicArrayMap&&) noexcept = default;

    ~BasicArrayMap() noexcept = default;

    /**
     * 通过 std::array 构造
     * @param init_arr 构造的数组类
     */
    explicit constexpr BasicArrayMap(const std::array<value_type, max_size>& init_arr) noexcept : store_array_(init_arr)
    {
    }

    /**
     * 通过 C 风格数组构造
     * @param init_arr C 风格数组
     */
    explicit constexpr BasicArrayMap(const value_type (&init_arr)[max_size]) noexcept
        : store_array_(std::to_array(init_arr))
    {
    }

    /**
     * 获取用于比较键是否相等的函数
     * @return 用于比较键是否相等的函数
     */
    constexpr KeyEqual key_eq() const noexcept
    {
        return key_equal_;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    /**
     * 获取最大存储的元素个数
     * @return 存储的元素个数
     */
    [[nodiscard]] constexpr size_type maxSize() const noexcept
    {
        return store_array_.size();
    }

    /**
     * 获取最大存储的大小（以字节为单位）
     * @return 存储的大小（以字节为单位）
     */
    [[nodiscard]] constexpr size_type maxSizeByte() const noexcept
    {
        return store_array_.size() * sizeof(mapped_type);
    }

    /**
     * 获取底层存储所用数组的指针
     * @return 底层存储所用数组的指针
     */
    [[nodiscard]] constexpr value_type* data() noexcept
    {
        return store_array_.data();
    }
    /**
     * @overload
     */
    [[nodiscard]] constexpr const value_type* data() const noexcept
    {
        return store_array_.data();
    }

private:
    /**
     * 在范围内查找特定的键
     * @tparam Iterator 迭代器的类型
     * @param key 待查找的键
     * @param begin_it 范围左区间
     * @param end_it 范围右区间
     * @return 查找到的键所在位置的迭代器。如果未查找到，返回 @ref end_it
     */
    template<typename Iterator>
    constexpr Iterator findInRangeGeneric_(const key_type& key, Iterator begin_it, Iterator end_it) const noexcept
    {
        for (auto it = begin_it; it != end_it; ++it) {
            if (key_equal_(it->first, key)) {
                return it;
            }
        }

        return end_it;
    }

protected:
    /**
     * 查找特定的键
     * @param key 待查找的键
     * @param l 范围左区间
     * @param r 范围右区间
     * @return 查找到的键所在位置的迭代器。如果未查找到，返回 @ref r
     */
    constexpr iterator findInRange_(const key_type& key, iterator l, iterator r) noexcept
    {
        return findInRangeGeneric_<iterator>(key, l, r);
    }
    /**
     * @overload
     */
    constexpr const_iterator findInRange_(const key_type& key, const_iterator l, const_iterator r) const noexcept
    {
        return findInRangeGeneric_<const_iterator>(key, l, r);
    }

    /**
     * 在范围内获取值
     * @attention 在非调试模式下没有范围检查
     * @param key 待获取值的键
     * @param l 范围左区间
     * @param r 范围右区间
     * @return 传入的键对应的值
     */
    constexpr mapped_type& getItemInRange_(const key_type& key, iterator l, iterator r) noexcept
    {
        auto it = findInRange_(key, l, r);
        EMDEVIF_ASSERT(it != r, "Out of range!");
        return it->second;
    }
    /**
     * @overload
     */
    constexpr const mapped_type& getItemInRange_(const key_type& key, const_iterator l, const_iterator r) const noexcept
    {
        auto it = findInRange_(key, l, r);
        EMDEVIF_ASSERT(it != r, "Out of range!");
        return it->second;
    }

    /**
     * 在范围内获取值 - 有范围检查
     * @param key 待获取值的键
     * @param l 范围左区间
     * @param r 范围右区间
     * @return 传入的键对应的值的指针
     */
    constexpr mapped_type* atInRange_(const key_type& key, iterator l, iterator r) noexcept
    {
        auto it = findInRange_(key, l, r);
        if (it == r) {
            return nullptr;
        }

        return &it->second;
    }
    /**
     * @overload
     */
    constexpr const mapped_type* atInRange_(const key_type& key, const_iterator l, const_iterator r) const noexcept
    {
        auto it = findInRange_(key, l, r);
        if (it == r) {
            return nullptr;
        }

        return &it->second;
    }

    /**
     * 断言辅助函数：断言不存在重复的键
     * @tparam Iterator 迭代器
     * @param l 范围左区间
     * @param r 范围右区间
     * @return 如果存在重复的键，返回 false
     */
    template<typename Iterator>
    [[nodiscard]] constexpr bool assertNoneRepeatKeys_(Iterator l, Iterator r) const noexcept
    {
        std::array<key_type, max_size> keys;
        size_type key_count = 0;

        auto hasKey = [this, &keys, &key_count](const key_type& key) noexcept {
            for (size_type i = 0; i < key_count; ++i) {
                if (key_equal_(keys[i], key)) {
                    return true;
                }
            }
            return false;
        };
        auto addKey = [&keys, &key_count](const key_type& key) noexcept { keys[key_count++] = key; };

        for (auto it = l; it != r; ++it) {
            if (hasKey(it->first)) {
                return false;
            }

            addKey(it->first);
        }

        return true;
    }
};

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_DATA_CONTAINER_ARRAY_MAP_BASIC_ARRAY_MAP_HPP
