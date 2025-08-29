/**
 * @file bit_int_base.cppm
 * @module emdevif.util.BitInt:base
 * @author DuYicheng
 * @date 2025-08-29
 * @brief 指定宽度整型公共函数
 */

module;

#include <cstdint>
#include <type_traits>
#include <concepts>

export module emdevif.util.BitInt:base;

namespace emdevif {

export using BitsType_t = uint_fast8_t;

export template<BitsType_t bits>
concept ValidBitIntWidth = (bits <= 64U);

namespace BitInt_internal {

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

}  // namespace BitInt_internal

}  // namespace emdevif
