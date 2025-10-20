/**
 * @file StaticMap.cppm
 * @author DuYicheng
 * @date 2025-09-04
 * @brief 静态键值对
 */

module;

#include <utility>
#include <array>
#include <algorithm>
#include <iterator>

export module emdevif.container.map:staticMap;

export namespace emdevif {

template<typename KeyType, typename ValueType, std::size_t max_size>
class StaticMap
{
public:
    using MapStoreType = std::pair<KeyType, ValueType>;

private:
    std::array<MapStoreType, max_size> store_array_;

public:
    StaticMap() = delete;

    constexpr explicit StaticMap(const std::array<MapStoreType, max_size>& init_arr) noexcept : store_array_(init_arr)
    {
    }

    constexpr StaticMap(const StaticMap& other) noexcept : store_array_(other.store_array_) {}

    constexpr auto cbegin() const noexcept
    {
        return store_array_.cbegin();
    }

    constexpr auto cend() const noexcept
    {
        return store_array_.cend();
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

    constexpr const ValueType& operator[](const KeyType& key) const noexcept
    {
        return find(key)->second;
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
consteval auto makeStaticMap(const std::pair<KeyType, ValueType> (&init_arr)[size]) noexcept
{
    std::array<std::pair<KeyType, ValueType>, size> arr;

    std::copy(init_arr, init_arr + size, arr.data());

    return StaticMap<KeyType, ValueType, size>(arr);
}

}  // namespace emdevif
