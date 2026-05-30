/**
 * @file basic_config.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_UTIL_BIT_INT_BASIC_CONFIG_HPP
    #define EMDEVIF_UTIL_BIT_INT_BASIC_CONFIG_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include <cstdint>
        #include <type_traits>
        #include <concepts>
    #endif

namespace emdevif {

/**
 * @brief 位宽类型，用于表示整数的位数
 */
EMDEVIF_MODULE_EXPORT using BitsType_t = uint_fast8_t;

/**
 * @brief 有效的 BitInt 位宽概念，限制位宽不超过 64
 */
EMDEVIF_MODULE_EXPORT
template<BitsType_t bits>
concept ValidBitIntWidth = (bits <= 64U);

namespace detail {

/**
 * @brief 获取整数类型对应的位宽
 * @tparam T 整数类型
 * @return 对应的位宽值（8/16/32/64），不支持的返回 0
 */
template<std::integral T>
consteval BitsType_t bitsOf() noexcept
{
    if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>) {
        return 8U;
    }
    else if constexpr (std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>) {
        return 16U;
    }
    else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t>) {
        return 32U;
    }
    else if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>) {
        return 64U;
    }
    else {
        return 0U;
    }
}

}  // namespace detail

}  // namespace emdevif

#endif  // !EMDEVIF_UTIL_BIT_INT_BASIC_CONFIG_HPP
