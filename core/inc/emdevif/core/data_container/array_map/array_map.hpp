/**
 * @file array_map.hpp
 * @brief 用数组实现的键值对 - 完整功能
 */

#pragma once
#ifndef EMDEVIF_CORE_DATA_CONTAINER_ARRAY_MAP_ARRAY_MAP_HPP
#define EMDEVIF_CORE_DATA_CONTAINER_ARRAY_MAP_ARRAY_MAP_HPP

#include <cstring>

#include <algorithm>
#include <array>
#include <concepts>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

#include "emdevif/core/fatal_handler.h"

#include "emdevif/core/error_handler.hpp"

namespace emdevif {

/**
 * 以数组存储的键值对容器
 * @tparam KeyType 键的类型
 * @tparam ValueType 值的类型
 * @tparam KeyEqual 用于比较键是否相等的函数类型
 * @tparam max_size 键值对最大数量
 */
template<typename KeyType, typename ValueType, std::size_t max_size, typename KeyEqual = std::equal_to<KeyType>>
class ArrayMap : public BasicArrayMap<KeyType, ValueType, max_size, KeyEqual>
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

private:
    size_type item_count{};  ///< 存储的元素个数

public:
    ArrayMap() noexcept = default;
    ArrayMap(const ArrayMap&) = default;
    ~ArrayMap() = default;

    /**
     * 通过 std::array 构造
     * @param init_arr 构造的数组类
     * @param real_size 实际构造的大小（默认值为 max_size）
     */
    explicit constexpr ArrayMap(const std::array<value_type, max_size>& init_arr,
                                size_type real_size = max_size) noexcept
        : SuperType_(init_arr), item_count(real_size)
    {
        EMDEVIF_ASSERT(real_size <= max_size);
        EMDEVIF_ASSERT(this->assertNoneRepeatKeys_(begin(), end()), "Have repeated keys!");
    }

    /**
     * 通过 C 风格数组构造
     * @param init_arr C 风格数组
     * @param real_size 实际构造的大小（默认值为 max_size）
     */
    explicit constexpr ArrayMap(const value_type (&init_arr)[max_size], size_type real_size = max_size) noexcept
        : ArrayMap(std::to_array(init_arr), real_size)
    {
    }

    /// 获取容器内存储的元素的数量
    [[nodiscard]] constexpr size_type size() const noexcept
    {
        return item_count;
    }

    /// 获取容器内存储的值的字节大小
    [[nodiscard]] constexpr size_type sizeByte() const noexcept
    {
        return size() * sizeof(mapped_type);
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
        return begin() + item_count;
    }
    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return begin() + item_count;
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return this->store_array_.cbegin();
    }
    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return cbegin() + item_count;
    }

    /**
     * 查找特定的键
     * @param key 待查找的键
     * @return 查找到的键所在位置的迭代器。如果待查找的键不存在，返回 end()。
     */
    [[nodiscard]] constexpr const_iterator find(const key_type& key) const noexcept
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

public:
    /// 判断当前容器是否为空
    [[nodiscard]] constexpr bool isEmpty() const noexcept
    {
        return item_count == 0;
    }

    /// 判断当前容器是否装满
    [[nodiscard]] constexpr bool isFull() const noexcept
    {
        return item_count == max_size;
    }

    /// 清除所有元素
    constexpr void clear() noexcept
    {
        item_count = 0;
    }

    /**
     * 向容器内插入元素（一个键值对）
     * @param v 插入的元素
     */
    constexpr void insert(const value_type& v) noexcept
    {
        auto it = this->findInRange_(v.first, begin(), end());
        if (it != end()) {
            it->second = v.second;
            return;
        }

        ++item_count;
        EMDEVIF_ASSERT(item_count <= max_size);

        *it = v;
    }
    /**
     * @overload
     * 向容器内移动元素（一个键值对）
     * @param v 插入的元素
     */
    constexpr void insert(value_type&& v) noexcept
    {
        auto it = this->findInRange_(v.first, begin(), end());
        if (it != end()) {
            it->second = v.second;
            return;
        }

        ++item_count;
        EMDEVIF_ASSERT(item_count <= max_size);

        *it = std::move(v);
    }

    /**
     * 擦除给定迭代器的元素
     * @attention 非调试模式下没有边界检查
     * @param pos 待擦除的元素的迭代器
     * @return 待擦除的元素的迭代器的下一个元素的迭代器
     */
    constexpr iterator erase(iterator pos) noexcept
    {
        EMDEVIF_ASSERT(pos < end(), "Out of range!");

        if constexpr (std::is_trivially_copyable_v<value_type>) {
            std::memmove(pos, pos + 1, std::distance(pos + 1, end()) * sizeof(value_type));
        }
        else {
            std::destroy_at(std::addressof(*pos));
            std::ranges::move(pos + 1, end(), pos);
        }

        --item_count;

        return pos;
    }
    /**
     * @overload
     * 擦除给定键的元素
     * @param key 待擦除的元素的键
     * @return 擦除的元素个数（0 或 1 个）
     */
    constexpr size_type erase(const key_type& key) noexcept
    {
        auto it = this->findInRange_(key, begin(), end());
        if (it != end()) {
            erase(it);
            return 1;
        }

        return 0;
    }
};

/**
 * 构造 @ref emdevif::ArrayMap 类
 * @tparam KeyType 键的类型
 * @tparam ValueType 值的类型
 * @tparam size 构造的 ArrayMap 类的键值对数量（应该由编译器推导值，不应该显示传值）
 * @tparam KeyEqual 用于比较键是否相等的函数类型
 * @param init_arr 初始化所用的 C 风格数组
 * @return 构造好的 ArrayMap 类
 */
template<typename KeyType, typename ValueType, std::size_t size, typename KeyEqual = std::equal_to<KeyType>>
constexpr auto makeArrayMap(const std::pair<KeyType, ValueType> (&init_arr)[size]) noexcept
{
    return ArrayMap<KeyType, ValueType, size, KeyEqual>(init_arr);
}
/**
 * @overload
 * 构造 @ref emdevif::ArrayMap 类（当初始化数组长度小于键值对长度时）
 * @tparam KeyType 键的类型
 * @tparam ValueType 值的类型
 * @tparam real_size 待构造的键值对的长度
 * @tparam KeyEqual 用于比较键是否相等的函数类型
 * @tparam arr_size 初始化数组长度（应该由编译器推导值，不应该显示传值）
 * @param init_arr 初始化所用的 C 风格数组
 * @return 构造好的 ArrayMap 类
 */
template<typename KeyType,
         typename ValueType,
         std::size_t real_size,
         typename KeyEqual = std::equal_to<KeyType>,
         std::size_t arr_size>
constexpr auto makeArrayMap(const std::pair<KeyType, ValueType> (&init_arr)[arr_size]) noexcept
{
    static_assert(arr_size <= real_size);

    std::pair<KeyType, ValueType> init_arr_cpy[real_size];
    std::ranges::copy_n(init_arr, arr_size, init_arr_cpy);

    return ArrayMap<KeyType, ValueType, real_size, KeyEqual>(init_arr_cpy, arr_size);
}
/**
 * @overload
 * 构造 @ref emdevif::ArrayMap 类（当初始化数组长度小于键值对长度时）
 * @tparam KeyType 键的类型
 * @tparam ValueType 值的类型
 * @tparam map_size 待构造的键值对的长度
 * @tparam KeyEqual 用于比较键是否相等的函数类型
 * @tparam arr_size 构造的 ArrayMap 类的键值对数量（应该由编译器推导值，不应该显示传值）
 * @param init_arr 初始化所用的 C 风格数组
 * @param real_size 实际构造的大小
 * @return 构造好的 ArrayMap 类
 */
template<typename KeyType,
         typename ValueType,
         std::size_t map_size,
         typename KeyEqual = std::equal_to<KeyType>,
         std::size_t arr_size>
constexpr auto makeArrayMap(const std::pair<KeyType, ValueType> (&init_arr)[arr_size], std::size_t real_size) noexcept
{
    static_assert(arr_size <= map_size);
    EMDEVIF_ASSERT(real_size <= arr_size);

    std::pair<KeyType, ValueType> init_arr_cpy[map_size];
    std::ranges::copy_n(init_arr, arr_size, init_arr_cpy);

    return ArrayMap<KeyType, ValueType, map_size, KeyEqual>(init_arr, real_size);
}

}  // namespace emdevif

namespace emdevif {

namespace detail {

/**
 * @brief 有效的 ArrayMap erase_if 谓词概念
 * @tparam Callable 可调用对象类型
 * @tparam Arg 参数类型
 */
template<typename Callable, typename Arg>
concept ValidArrayMapEraseIfPred = requires(Callable callable, const Arg& arg) {
    { callable(arg) } -> std::same_as<bool>;
};

}  // namespace detail

/**
 * 移除满足谓词条件的元素
 * @tparam KeyType 键的类型
 * @tparam ValueType 值的类型
 * @tparam max_size 键值对最大数量
 * @tparam KeyEqual 用于比较键是否相等的函数类型
 * @tparam Pred 谓词类型
 * @param c 待操作的 ArrayMap
 * @param pred 谓词函数
 * @return 移除的元素个数
 */
template<typename KeyType,
         typename ValueType,
         std::size_t max_size,
         typename KeyEqual = std::equal_to<KeyType>,
         typename Pred>
    requires detail::ValidArrayMapEraseIfPred<Pred,
                                              typename ArrayMap<KeyType, ValueType, max_size, KeyEqual>::value_type>
std::size_t erase_if(ArrayMap<KeyType, ValueType, max_size, KeyEqual>& c, Pred pred)
{
    auto old_size = c.size();

    for (auto it = c.begin(); it != c.end();) {
        if (pred(*it)) {
            it = c.erase(it);
        }
        else {
            ++it;
        }
    }

    return old_size - c.size();
}

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_DATA_CONTAINER_ARRAY_MAP_ARRAY_MAP_HPP
