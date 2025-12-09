/**
 * @file PeripheralHandleMap.cppm
 * @brief
 */

module;

#include <string_view>
#include <type_traits>

#include "emdevif/simplify_decl_macros.hpp"

export module emdevif.peripheralHandleMap;

import emdevif.typeTraits;
import emdevif.userDeclares;
import emdevif.container.map;

namespace emdevif {

static_assert(emdevif::is_consteval([] { return user_declares::peripheral_handle_map; }),
              "Variable `peripheral_handle_map\' should be a constant expression.");
static_assert(
    std::is_same_v<decltype(user_declares::peripheral_handle_map)::value_type, std::pair<std::string_view, void*>>,
    "Variable `peripheral_handle_map\' should be declared as `constexpr auto peripheral_handle_map = "
    "makeStaticMap<std::string_view, void*>\'.");

export class PeripheralHandleMap
{
public:
    PeripheralHandleMap() = delete;
    ~PeripheralHandleMap() = delete;
    EMDEVIF_DELETE_COPY_AND_MOVE(PeripheralHandleMap);

    static constexpr void* findHandle(const std::string_view name) noexcept
    {
        const auto handle = user_declares::peripheral_handle_map.at(name);

        if (handle == nullptr) {
            return nullptr;
        }

        return *handle;
    }
};

}  // namespace emdevif
