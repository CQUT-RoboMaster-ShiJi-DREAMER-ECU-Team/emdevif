/**
 * @file ArrayMap.cppm
 * @author DuYicheng
 * @date 2025-09-04
 * @brief 用数组实现的键值对
 */

module;

#include <utility>
#include <array>
#include <algorithm>
#include <iterator>

export module emdevif.container.map:arrayMap;

export namespace emdevif {

// todo 待实现插入/删除
// 考虑增加记录数组顶部的指针
/**
 * 以数组存储的键值对
 * @tparam KeyType 键的类型
 * @tparam ValueType 值的类型
 * @tparam max_size 键值对最大数量
 */
template<typename KeyType, typename ValueType, std::size_t max_size>
class ArrayMap
{
public:
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

private:
    std::array<value_type, max_size> store_array_;  ///< 内部存储所用的数组

public:
    constexpr ArrayMap() noexcept = default;
    constexpr ArrayMap(const ArrayMap&) noexcept = default;
    constexpr ArrayMap(ArrayMap&&) noexcept = default;

    ~ArrayMap() noexcept = default;

    /**
     * 通过 std::array 构造
     * @param init_arr 构造的数组类
     */
    explicit constexpr ArrayMap(const std::array<value_type, max_size>& init_arr) noexcept : store_array_(init_arr) {}

    /**
     * 通过 C 风格数组构造
     * @param init_arr C 风格数组
     */
    explicit constexpr ArrayMap(const value_type (&init_arr)[max_size]) noexcept : store_array_(std::to_array(init_arr))
    {
    }
    /**
     * @overload
     * 通过 C 风格数组移动构造
     * @param init_arr C 风格数组
     */
    explicit constexpr ArrayMap(const value_type (&&init_arr)[max_size]) noexcept
        : store_array_(std::to_array(std::move(init_arr)))
    {
    }

    constexpr iterator begin() noexcept
    {
        return store_array_.begin();
    }
    constexpr const_iterator begin() const noexcept
    {
        return store_array_.begin();
    }

    constexpr iterator end() noexcept
    {
        return store_array_.end();
    }
    constexpr const_iterator end() const noexcept
    {
        return store_array_.end();
    }

    constexpr const_iterator cbegin() const noexcept
    {
        return store_array_.cbegin();
    }

    constexpr const_iterator cend() const noexcept
    {
        return store_array_.cend();
    }

    /**
     * 获取存储的元素个数
     * @return 存储的元素个数
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    [[nodiscard]] constexpr size_type size() const noexcept
    {
        return max_size;
    }

    /**
     * 获取存储的大小（以字节为单位）
     * @return 存储的大小（以字节为单位）
     */
    [[nodiscard]] constexpr size_type size_byte() const noexcept
    {
        return max_size * store_array_.size() * sizeof(value_type);
    }

    /**
     * 查找特定的键
     * @param key 待查找的键
     * @return 查找到的键所在位置的迭代器。如果待查找的键不存在，返回 end()。
     */
    constexpr iterator find(const KeyType& key) noexcept
    {
        for (auto it = store_array_.begin(); it != store_array_.end(); ++it) {
            if (it->first == key) {
                return it;
            }
        }

        return store_array_.end();
    }
    /**
     * @overload
     */
    constexpr const_iterator find(const KeyType& key) const noexcept
    {
        for (auto it = store_array_.cbegin(); it != store_array_.cend(); ++it) {
            if (it->first == key) {
                return it;
            }
        }

        return store_array_.cend();
    }

    /**
     * 获取给定键的映射值
     * @attention 无边界检查。如果键不存在，属于未定义行为。
     * @param key 待获取映射后的值的键
     * @return 给定键的对应值
     */
    constexpr mapped_type& operator[](const KeyType& key) noexcept
    {
        return find(key)->second;
    }
    /**
     * @overload
     */
    constexpr const mapped_type& operator[](const KeyType& key) const noexcept
    {
        return find(key)->second;
    }

    /**
     * 获取给定键的映射值的指针
     * @param key 待获取映射后的值的键
     * @return 给定键的对应值的指针。如果键不存在，返回 nullptr
     */
    constexpr mapped_type* at(const KeyType& key) noexcept
    {
        auto it = find(key);

        if (it == end()) {
            return nullptr;
        }

        return &(it->second);
    }
    /**
     * @overload
     */
    constexpr const mapped_type* at(const KeyType& key) const noexcept
    {
        auto it = find(key);

        if (it == cend()) {
            return nullptr;
        }

        return &(it->second);
    }
};

/**
 * 构造 @ref emdevif::ArrayMap 类
 * @tparam KeyType 键的类型
 * @tparam ValueType 值的类型
 * @tparam size 构造的 ArrayMap 类的键值对数量
 * @param init_arr 初始化所用的 C 风格数组
 * @return 构造好的 ArrayMap 类
 */
template<typename KeyType, typename ValueType, std::size_t size>
consteval auto makeArrayMap(const std::pair<KeyType, ValueType> (&init_arr)[size]) noexcept
{
    return ArrayMap<KeyType, ValueType, size>(init_arr);
}

/**
 * 静态键值对
 * @tparam KeyType 键的类型
 * @tparam ValueType 值的类型
 * @tparam max_size 键值对最大数量
 */
template<typename KeyType, typename ValueType, std::size_t max_size>
class StaticMap : public ArrayMap<KeyType, ValueType, max_size>
{
public:
    using value_type = typename ArrayMap<KeyType, ValueType, max_size>::value_type;

    consteval StaticMap() noexcept = default;
    consteval StaticMap(const StaticMap&) noexcept = default;
    consteval StaticMap(StaticMap&&) noexcept = default;

    ~StaticMap() noexcept = default;

    /**
     * 通过 std::array 类构造，用法与 @ref emdevif::ArrayMap 的构造函数相同
     */
    explicit consteval StaticMap(const std::array<value_type, max_size>& init_arr) noexcept
        : ArrayMap<KeyType, ValueType, max_size>(init_arr)
    {
    }
    /**
     * @overload
     */
    explicit consteval StaticMap(const value_type (&init_arr)[max_size]) noexcept
        : ArrayMap<KeyType, ValueType, max_size>(init_arr)
    {
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
template<typename KeyType, typename ValueType, std::size_t size>
consteval auto makeStaticMap(const std::pair<KeyType, ValueType> (&init_arr)[size]) noexcept
{
    return StaticMap<KeyType, ValueType, size>{init_arr};
}

}  // namespace emdevif
