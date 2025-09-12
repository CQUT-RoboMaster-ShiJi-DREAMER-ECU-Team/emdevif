/**
 * @file PeripheralHandleMap.cppm
 * @module emdevif.PeripheralHandleMap
 * @author DuYicheng
 * @date 2025-09-04
 * @brief
 */

module;

#include <string_view>
#include <optional>
#include <type_traits>

export module emdevif.peripheralHandleMap;

import emdevif.from_user_impl;

namespace emdevif {

static_assert(
    std::is_same_v<decltype(from_user_impl::peripheral_handle_map)::MapStoreType, std::pair<std::string_view, void*>>);

export class PeripheralHandleMap
{
public:
    constexpr static std::optional<void*> findHandle(const std::string_view name)
    {
        const auto handle = from_user_impl::peripheral_handle_map.at(name);
        if (handle == nullptr) {
            return std::nullopt;
        }

        return *handle;
    }
};

}  // namespace emdevif
