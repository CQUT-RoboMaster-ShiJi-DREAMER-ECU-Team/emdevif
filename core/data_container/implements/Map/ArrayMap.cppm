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
template<typename KeyType, typename ValueType, std::size_t max_size>
class ArrayMap
{
public:
    using MapStoreType = std::pair<KeyType, ValueType>;

private:
    std::array<MapStoreType, max_size> store_array_;

public:
    constexpr ArrayMap() noexcept = default;
    constexpr ArrayMap(const ArrayMap&) noexcept = default;
    constexpr ArrayMap(ArrayMap&&) noexcept = default;

    ~ArrayMap() noexcept = default;

    explicit constexpr ArrayMap(const std::array<MapStoreType, max_size>& init_arr) noexcept : store_array_(init_arr) {}
    explicit constexpr ArrayMap(const MapStoreType (&init_arr)[max_size]) noexcept
        : store_array_(std::to_array(init_arr))
    {
    }
    explicit constexpr ArrayMap(const MapStoreType (&&init_arr)[max_size]) noexcept
        : store_array_(std::to_array(std::move(init_arr)))
    {
    }

    constexpr auto begin() noexcept
    {
        return store_array_.begin();
    }
    constexpr auto begin() const noexcept
    {
        return store_array_.begin();
    }

    constexpr auto end() noexcept
    {
        return store_array_.end();
    }
    constexpr auto end() const noexcept
    {
        return store_array_.end();
    }

    constexpr auto cbegin() noexcept
    {
        return store_array_.cbegin();
    }
    constexpr auto cbegin() const noexcept
    {
        return store_array_.cbegin();
    }

    constexpr auto cend() noexcept
    {
        return store_array_.cend();
    }
    constexpr auto cend() const noexcept
    {
        return store_array_.cend();
    }

    constexpr auto find(const KeyType& key) noexcept
    {
        for (auto it = store_array_.begin(); it != store_array_.end(); ++it) {
            if (it->first == key) {
                return it;
            }
        }

        return store_array_.end();
    }
    constexpr auto find(const KeyType& key) const noexcept
    {
        for (auto it = store_array_.cbegin(); it != store_array_.cend(); ++it) {
            if (it->first == key) {
                return it;
            }
        }

        return store_array_.cend();
    }

    constexpr ValueType& operator[](const KeyType& key) noexcept
    {
        return find(key)->second;
    }
    constexpr const ValueType& operator[](const KeyType& key) const noexcept
    {
        return find(key)->second;
    }

    constexpr ValueType* at(const KeyType& key) noexcept
    {
        auto it = find(key);

        if (it == end()) {
            return nullptr;
        }

        return &(it->second);
    }
    constexpr const ValueType* at(const KeyType& key) const noexcept
    {
        auto it = find(key);

        if (it == cend()) {
            return nullptr;
        }

        return &(it->second);
    }
};

template<typename KeyType, typename ValueType, std::size_t size>
consteval auto makeArrayMap(const std::pair<KeyType, ValueType> (&init_arr)[size]) noexcept
{
    return ArrayMap<KeyType, ValueType, size>(init_arr);
}

template<typename KeyType, typename ValueType, std::size_t max_size>
class StaticMap : public ArrayMap<KeyType, ValueType, max_size>
{
public:
    using MapStoreType = typename ArrayMap<KeyType, ValueType, max_size>::MapStoreType;

    consteval StaticMap() noexcept = default;
    consteval StaticMap(const StaticMap&) noexcept = default;
    consteval StaticMap(StaticMap&&) noexcept = default;

    ~StaticMap() noexcept = default;

    explicit consteval StaticMap(const std::array<MapStoreType, max_size>& init_arr) noexcept
        : ArrayMap<KeyType, ValueType, max_size>(init_arr)
    {
    }
    explicit consteval StaticMap(const MapStoreType (&init_arr)[max_size]) noexcept
        : ArrayMap<KeyType, ValueType, max_size>(init_arr)
    {
    }
};

template<typename KeyType, typename ValueType, std::size_t size>
consteval auto makeStaticMap(const std::pair<KeyType, ValueType> (&init_arr)[size]) noexcept
{
    return StaticMap<KeyType, ValueType, size>{init_arr};
}

}  // namespace emdevif
