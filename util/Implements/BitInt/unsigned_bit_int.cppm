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
#include <iostream>

#include "emdevif/concepts.hpp"

#include "BitInt_exception_config.hpp"

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

    template<std::integral OtherType>  // NOLINTNEXTLINE(*-explicit-constructor)
    constexpr UBitInt(const OtherType other) noexcept : value(overflowCast(static_cast<RealType>(other)))
    {
    }

    template<std::integral OtherType>
    constexpr UBitInt& operator=(const OtherType other) noexcept
    {
        value = overflowCast(static_cast<RealType>(other));

        return *this;
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>  // NOLINTNEXTLINE(*-explicit-constructor)
    constexpr UBitInt(const UBitInt<other_bits>& other) noexcept : value(overflowCast(static_cast<RealType>(other)))
    {
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>
    constexpr UBitInt& operator=(const UBitInt<other_bits>& other) noexcept
    {
        value = overflowCast(static_cast<RealType>(other));

        return *this;
    }

    // ---------------------------- 一元 ---------------------------------
    // =========================== ~

    constexpr RealType operator~() const noexcept
    {
        return overflowCast(~value);
    }

    // =========================== !

    constexpr bool operator!() const noexcept
    {
        return !static_cast<bool>(overflowCast(value));
    }

    // =========================== +/-

    constexpr RealType operator+() const noexcept
    {
        return RealType(*this);
    }
    constexpr RealType operator-() const noexcept
    {
        return overflowCast(-value);
    }

    // ---------------------------- 二元 ---------------------------------
    // =========================== +

    template<typename OtherType>
    constexpr RealType operator+(const OtherType& other) const noexcept
    {
        return overflowCast(this->value + static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr RealType operator+(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return overflowCast(static_cast<RealType>(lhs) + rhs.value);
    }

    // =========================== -

    template<typename OtherType>
    constexpr RealType operator-(const OtherType& other) const noexcept
    {
        return overflowCast(this->value - static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr RealType operator-(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return overflowCast(static_cast<RealType>(lhs) - rhs.value);
    }

    // =========================== *

    template<typename OtherType>
    constexpr RealType operator*(const OtherType& other) const noexcept
    {
        return overflowCast(this->value * static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr RealType operator*(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return overflowCast(static_cast<RealType>(lhs) * rhs.value);
    }

    // =========================== /

    template<typename OtherType>
    constexpr RealType operator/(const OtherType& other) const EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
        const auto other_val = static_cast<RealType>(other);

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        if (other_val == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
#endif

        return overflowCast(this->value / other_val);
    }

    template<ArithmeticType OtherType>
    friend constexpr RealType operator/(const OtherType& lhs, const UBitInt& rhs) EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
        const auto rhs_val = static_cast<RealType>(rhs.value);

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        if (rhs_val == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
#endif

        return overflowCast(static_cast<RealType>(lhs) / rhs_val);
    }

    // =========================== %

    template<typename OtherType>
    constexpr RealType operator%(const OtherType& other) const EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
        const auto other_val = static_cast<RealType>(other);

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        if (other_val == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
#endif

        return overflowCast(this->value % other_val);
    }

    template<ArithmeticType OtherType>
    friend constexpr RealType operator%(const OtherType& lhs, const UBitInt& rhs) EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
        const auto rhs_val = static_cast<RealType>(rhs.value);

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        if (rhs_val == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
#endif

        return overflowCast(static_cast<RealType>(lhs) % rhs_val);
    }

    // =========================== &

    template<typename OtherType>
    constexpr RealType operator&(const OtherType& other) const noexcept
    {
        return overflowCast(this->value & static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr RealType operator&(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return overflowCast(static_cast<RealType>(lhs) & rhs.value);
    }

    // =========================== |

    template<typename OtherType>
    constexpr RealType operator|(const OtherType& other) const noexcept
    {
        return overflowCast(this->value | static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr RealType operator|(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return overflowCast(static_cast<RealType>(lhs) | rhs.value);
    }

    // =========================== ^

    template<typename OtherType>
    constexpr RealType operator^(const OtherType& other) const noexcept
    {
        return overflowCast(this->value ^ static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr RealType operator^(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return overflowCast(static_cast<RealType>(lhs) ^ rhs.value);
    }

    // =========================== <<

    template<std::integral OtherType>
    constexpr RealType operator<<(const OtherType shift) const noexcept
    {
        return overflowCast(value << shift);
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>
    constexpr RealType operator<<(const UBitInt<other_bits> shift) const noexcept
    {
        return overflowCast(value << RealType(shift));
    }

    // =========================== >>

    template<std::integral OtherType>
    constexpr RealType operator>>(const OtherType shift) const noexcept
    {
        return overflowCast(value >> shift);
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>
    constexpr RealType operator>>(const UBitInt<other_bits> shift) const noexcept
    {
        return overflowCast(value >> RealType(shift));
    }

    // ---------------------------- 复合赋值 -------------------------------
    // =========================== +=

    template<typename OtherType>
    constexpr UBitInt& operator+=(const OtherType& other) noexcept
    {
        return *this = *this + other;
    }

    // =========================== -=

    template<typename OtherType>
    constexpr UBitInt& operator-=(const OtherType& other) noexcept
    {
        return *this = *this - other;
    }

    // =========================== *=

    template<typename OtherType>
    constexpr UBitInt& operator*=(const OtherType& other) noexcept
    {
        return *this = *this * other;
    }

    // =========================== /=

    template<typename OtherType>
    constexpr UBitInt& operator/=(const OtherType& other) EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        RealType res;

        try {
            res = *this / other;
        }
        catch (std::invalid_argument&) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }

        return *this = res;
#else
        return *this = *this / other;
#endif
    }

    // =========================== %=

    template<typename OtherType>
    constexpr UBitInt& operator%=(const OtherType& other) EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        RealType res;

        try {
            res = *this % other;
        }
        catch (std::invalid_argument&) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }

        return *this = res;
#else
        return *this = *this % other;
#endif
    }

    // =========================== &=

    template<typename OtherType>
    constexpr UBitInt& operator&=(const OtherType& other) noexcept
    {
        return *this = *this & other;
    }

    // =========================== |=

    template<typename OtherType>
    constexpr UBitInt& operator|=(const OtherType& other) noexcept
    {
        return *this = *this | other;
    }

    // =========================== ^=

    template<typename OtherType>
    constexpr UBitInt& operator^=(const OtherType& other) noexcept
    {
        return *this = *this ^ other;
    }

    // =========================== <<=

    template<std::integral OtherType>
    constexpr UBitInt& operator<<=(const OtherType shift) const noexcept
    {
        return *this = *this << shift;
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>
    constexpr UBitInt& operator<<=(const UBitInt<other_bits> shift) const noexcept
    {
        return *this = *this << shift;
    }

    // =========================== >>=

    template<std::integral OtherType>
    constexpr UBitInt& operator>>=(const OtherType shift) const noexcept
    {
        return *this = *this >> shift;
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>
    constexpr UBitInt& operator>>=(const UBitInt<other_bits> shift) const noexcept
    {
        return *this = *this >> shift;
    }

    // ---------------------------- 自增/自减 ----------------------------
    // =========================== ++

    constexpr RealType operator++() noexcept
    {
        return *this += 1;
    }
    constexpr RealType operator++(int) noexcept
    {
        const auto tmp = *this;
        ++(*this);
        return tmp;
    }

    // =========================== --

    constexpr RealType operator--() noexcept
    {
        return *this -= 1;
    }
    constexpr RealType operator--(int) noexcept
    {
        const auto tmp = *this;
        --(*this);
        return tmp;
    }

    // ---------------------------- 比较 ----------------------------
    // =========================== ==

    template<typename OtherType>
    constexpr bool operator==(const OtherType& other) const noexcept
    {
        return (value == static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator==(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return (static_cast<RealType>(lhs) == rhs.value);
    }

    // =========================== !=

    template<typename OtherType>
    constexpr bool operator!=(const OtherType& other) const noexcept
    {
        return (value != static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator!=(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return (static_cast<RealType>(lhs) != rhs.value);
    }

    // =========================== >

    template<typename OtherType>
    constexpr bool operator>(const OtherType& other) const noexcept
    {
        return (value > static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator>(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return (static_cast<RealType>(lhs) > rhs.value);
    }

    // =========================== <

    template<typename OtherType>
    constexpr bool operator<(const OtherType& other) const noexcept
    {
        return (value < static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator<(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return (static_cast<RealType>(lhs) < rhs.value);
    }

    // =========================== >=

    template<typename OtherType>
    constexpr bool operator>=(const OtherType& other) const noexcept
    {
        return (value >= static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator>=(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return (static_cast<RealType>(lhs) >= rhs.value);
    }

    // =========================== <=

    template<typename OtherType>
    constexpr bool operator<=(const OtherType& other) const noexcept
    {
        return (value <= static_cast<RealType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator<=(const OtherType& lhs, const UBitInt& rhs) noexcept
    {
        return (static_cast<RealType>(lhs) <= rhs.value);
    }

    // --------------------------- stream ------------------------------

    inline friend std::ostream& operator<<(std::ostream& os, const UBitInt& bit_int)
    {
        os << static_cast<RealType>(bit_int);
        return os;
    }

    // end operators ---------------------------------------------------

    template<std::integral OtherType>
    operator OtherType() const noexcept  // NOLINT(*-explicit-constructor)
    {
        return static_cast<OtherType>(overflowCast(value));
    }

    constexpr ~UBitInt() noexcept = default;

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

    inline friend std::ostream& operator<<(std::ostream& os, const UBitInt&)
    {
        os << 0;
        return os;
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
