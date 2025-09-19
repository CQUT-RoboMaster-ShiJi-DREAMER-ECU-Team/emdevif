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

#include "emdevif/simplify_decl_macros.hpp"

export module emdevif.peripheralHandleMap;

import emdevif.userDeclares;

namespace emdevif {

static_assert(
    std::is_same_v<decltype(user_declares::peripheral_handle_map)::MapStoreType, std::pair<std::string_view, void*>>,
    "Variable `peripheral_handle_map\' should be declared as `constexpr auto peripheral_handle_map = "
    "makeStaticMap<std::string_view, void*>\'");

export class PeripheralHandleMap
{
public:
    PeripheralHandleMap() = delete;
    ~PeripheralHandleMap() = delete;
    EMDEVIF_DELETE_COPY_AND_MOVE(PeripheralHandleMap);

    static constexpr std::optional<void*> findHandle(const std::string_view name) noexcept
    {
        const auto handle = user_declares::peripheral_handle_map.at(name);
        if (handle == nullptr) {
            return std::nullopt;
        }

        return *handle;
    }
};

}  // namespace emdevif
