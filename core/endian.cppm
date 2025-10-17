/**
 * @file endian.cppm
 * @module emdevif.endian
 * @author DuYicheng
 * @date 2025-10-16
 * @brief 端序
 */

// ReSharper disable CppDFAUnreachableCode
// ReSharper disable CppDFAConstantFunctionResult

module;

#include <cstdint>

#include <concepts>
#include <bit>
#include <array>
#include <algorithm>

export module emdevif.endian;

namespace emdevif {

namespace internal {

template<typename T>
constexpr T byteSwapGeneric(T value) noexcept
{
    static_assert(std::has_unique_object_representations_v<T>, "T may not have padding bits");

    auto value_representation = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
    std::ranges::reverse(value_representation);
    return std::bit_cast<T>(value_representation);
}

template<std::integral T>
constexpr T byteSwapIntegral(const T v) noexcept
{
#ifdef __cpp_lib_byteswap
    return std::byteswap(v);
#else

    if constexpr (sizeof(T) == 1) {
        return v;
    }

    if constexpr (sizeof(T) == 2) {
        return ((((v) >> 8) & 0xFFU) | (((v) & 0xFFU) << 8));
    }
    if constexpr (sizeof(T) == 4) {
        return ((((v) & 0xFF000000U) >> 24) | (((v) & 0x00FF0000U) >> 8) | (((v) & 0x0000FF00U) << 8) |
                (((v) & 0x000000FFU) << 24));
    }
    if constexpr (sizeof(T) == 8) {
        return ((((v) & 0xFF00000000000000ULL) >> 56) | (((v) & 0x00FF000000000000ULL) >> 40) |
                (((v) & 0x0000FF0000000000ULL) >> 24) | (((v) & 0x000000FF00000000ULL) >> 8) |
                (((v) & 0x00000000FF000000ULL) << 8) | (((v) & 0x0000000000FF0000ULL) << 24) |
                (((v) & 0x000000000000FF00ULL) << 40) | (((v) & 0x00000000000000FFULL) << 56));
    }

    return byteSwapGeneric(v);
#endif
}

}  // namespace internal

export template<typename T>
constexpr T byteSwap(const T v) noexcept
{
    if constexpr (sizeof(T) == 1) {
        return v;
    }

    constexpr std::array integral_support_size_list{2, 4, 8};
    if constexpr (std::ranges::find(integral_support_size_list, sizeof(T))) {
        if constexpr (!std::is_integral_v<T>) {
            constexpr auto size_of_T = sizeof(T);
            /* clang-format off */
            using ConvertIntegral =
                std::conditional_t<(size_of_T <= 1U), uint8_t,
                    std::conditional_t<(size_of_T >= 2U && size_of_T <= 3U), uint16_t,
                        std::conditional_t<(size_of_T >= 4U && size_of_T <= 7U), uint32_t,
                            uint64_t
                        >
                    >
                >;
            /* clang-format on */

            auto u = std::bit_cast<ConvertIntegral>(v);
            u = internal::byteSwapIntegral(u);
            return std::bit_cast<T>(u);
        }
        else {
            return internal::byteSwapIntegral(v);
        }
    }

    return internal::byteSwapGeneric(v);
}

}  // namespace emdevif
