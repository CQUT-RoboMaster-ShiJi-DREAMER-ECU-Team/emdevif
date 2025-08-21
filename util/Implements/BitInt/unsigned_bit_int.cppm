/**
 * @file unsigned_bit_int.cppm
 * @module emdevif.util.BitInt:unsigned_partial
 * @author DuYicheng
 * @date 2025-08-16
 * @brief 无符号指定宽度整型
 */

// ReSharper disable CppNonExplicitConversionOperator
// ReSharper disable CppNonExplicitConvertingConstructor

module;

#include <cstdint>
#include <type_traits>
#include <concepts>

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
#include <stdexcept>

#define EMDEVIF_UTIL_BITINT_NOEXCEPT
#else
#define EMDEVIF_UTIL_BITINT_NOEXCEPT noexcept
#endif

export module emdevif.util.BitInt:unsigned_partial;
import :signed_partial;

export namespace emdevif {

template<BitsType_t bits>
    requires ValidBitIntWidth<bits>
class UBitInt
{
protected:  // for testing
    /* clang-format off */

    using RealType =
        std::conditional_t<(bits <= 8U), uint8_t,
            std::conditional_t<(bits > 8U && bits <= 16U), uint16_t,
                std::conditional_t<(bits > 16U && bits <= 32U), uint32_t,
                    uint64_t
                >
            >
        >;

    /* clang-format on */

    static constexpr RealType overflowCast(const RealType value) noexcept
    {
        if constexpr (bits == bitsOf<RealType>()) {
            return value;
        }
        else {
            return value & ((RealType(1) << RealType(bits)) - RealType(1));
        }
    }

public:
    static constexpr RealType max() noexcept
    {
        if constexpr (bits == bitsOf<RealType>()) {
            return std::numeric_limits<RealType>::max();
        }
        else {
            return ((RealType(1) << RealType(bits)) - RealType(1));
        }
    }

    static constexpr RealType min() noexcept
    {
        return 0U;
    }

    constexpr UBitInt() noexcept = default;

private:
    RealType value;
};

template<>
class UBitInt<0>
{
public:
    static constexpr uint8_t max() noexcept
    {
        return 0;
    }

    static constexpr uint8_t min() noexcept
    {
        return 0;
    }

    // ReSharper disable once CppDFAConstantFunctionResult
    template<std::integral Type>
    operator Type() const noexcept  // NOLINT(*-explicit-constructor)
    {
        return static_cast<Type>(value);
    }

private:
    const uint8_t value{0U};
};

}  // namespace emdevif
