/**
 * @file peripheral_handle_map.hpp
 * @brief 外设句柄映射查找工具，提供编译期外设句柄查表功能
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_PERIPHERAL_HANDLE_MAP_HPP
    #define EMDEVIF_PERIPHERAL_PERIPHERAL_HANDLE_MAP_HPP


        #include "emdevif/core/detail/include_emdevif_user_declares_in_headers.hpp"

        #include "emdevif/core/type_traits.hpp"
        #include "emdevif/core/data_container/array_map.hpp"

        #include <string_view>
        #include <type_traits>

        #include "emdevif/core/simplify_decl_macros.hpp"

namespace emdevif {

static_assert(emdevif::is_consteval([] { return user_declares::peripheral_handle_map; }),
              "Variable `peripheral_handle_map\' should be a constant expression.");
static_assert(
    std::is_same_v<decltype(user_declares::peripheral_handle_map)::value_type, std::pair<std::string_view, void*>>,
    "Variable `peripheral_handle_map\' should be declared as `constexpr auto peripheral_handle_map = "
    "makeStaticMap<std::string_view, void*>\'.");

/// @brief 外设句柄映射查找工具类，通过编译期静态映射表根据名称查找外设句柄
class PeripheralHandleMap
{
public:
    PeripheralHandleMap() = delete;
    ~PeripheralHandleMap() = delete;
    EMDEVIF_DELETE_COPY_AND_MOVE(PeripheralHandleMap);

    /**
     * @brief 根据名称查找外设句柄
     * @param name 外设句柄名称
     * @return 外设句柄指针，未找到时返回 nullptr
     */
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
