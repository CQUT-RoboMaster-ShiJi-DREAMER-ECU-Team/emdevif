/**
 * @file peripheral_handle_map.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_PERIPHERAL_HANDLE_MAP_HPP
    #define EMDEVIF_PERIPHERAL_PERIPHERAL_HANDLE_MAP_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/core/detail/include_emdevif_user_declares_in_headers.hpp"

        #include "emdevif/core/type_traits.hpp"
        #include "emdevif/core/data_container/array_map.hpp"

        #include <string_view>
        #include <type_traits>

        #include "emdevif/core/simplify_decl_macros.hpp"
    #endif

namespace emdevif {

static_assert(emdevif::is_consteval([] { return user_declares::peripheral_handle_map; }),
              "Variable `peripheral_handle_map\' should be a constant expression.");
static_assert(
    std::is_same_v<decltype(user_declares::peripheral_handle_map)::value_type, std::pair<std::string_view, void*>>,
    "Variable `peripheral_handle_map\' should be declared as `constexpr auto peripheral_handle_map = "
    "makeStaticMap<std::string_view, void*>\'.");

EMDEVIF_MODULE_EXPORT class PeripheralHandleMap
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

#endif  // !EMDEVIF_PERIPHERAL_PERIPHERAL_HANDLE_MAP_HPP
