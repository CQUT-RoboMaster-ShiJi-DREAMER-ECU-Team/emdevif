/**
 * @file signed_bit_int.cppm
 * @module emdevif.util.BitInt:signed_partial
 * @author DuYicheng
 * @date 2025-08-16
 * @brief 有符号指定宽度整型
 */

// ReSharper disable CppNonExplicitConversionOperator
// ReSharper disable CppNonExplicitConvertingConstructor

module;

#include <cstdint>

#include <bit>
#include <numeric>
#include <concepts>
#include <iostream>

#include "emdevif/concepts.hpp"

#include "BitInt_exception_config.hpp"

export module emdevif.util.bitInt:signed_partial;
import :base;

export namespace emdevif {

template<BitsType_t bits>
    requires ValidBitIntWidth<bits>
class BitInt
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

    using SignedType =
        std::conditional_t<(bits <= 8U), int8_t,
            std::conditional_t<(bits > 8U && bits <= 16U), int16_t,
                std::conditional_t<(bits > 16U && bits <= 32U), int32_t,
                    int64_t
                >
            >
        >;

    /* clang-format on */

    static_assert(sizeof(SignedType) == sizeof(RealType));
    static_assert(BitInt_internal::bitsOf<RealType>() != 0);

    static constexpr RealType truncateToReal(const SignedType v) noexcept
    {
        if constexpr (bits == BitInt_internal::bitsOf<RealType>()) {
            return std::bit_cast<RealType>(v);
        }
        else {
            return static_cast<RealType>(v) & ((RealType(1) << bits) - 1);
        }
    }

    static constexpr SignedType transToSigned(const RealType v) noexcept
    {
        if constexpr (bits == BitInt_internal::bitsOf<RealType>()) {
            return std::bit_cast<SignedType>(v);
        }
        else {
            constexpr auto shift = BitInt_internal::bitsOf<RealType>() - bits;
            return (static_cast<SignedType>(v << shift)) >> shift;
        }
    }

    static constexpr SignedType bitsSetValue(const BitsType_t value_bits) noexcept
    {
        SignedType result = 0U;

        for (BitsType_t i = 0U; i < value_bits; ++i) {
            result |= (static_cast<SignedType>(1) << i);
        }

        return result;
    }

public:
    static constexpr SignedType max() noexcept
    {
        return bitsSetValue(bits - 1);
    }

    static constexpr SignedType min() noexcept
    {
        if constexpr (bits == 64) {
            return std::numeric_limits<int64_t>::min();
        }
        else {
            return -static_cast<int64_t>(RealType(1) << RealType(bits - 1));
        }
    }

    constexpr BitInt() noexcept = default;

    template<std::integral OtherType>  // NOLINTNEXTLINE(*-explicit-constructor)
    constexpr BitInt(const OtherType other) noexcept : value(truncateToReal(other))
    {
    }

    template<std::integral OtherType>
    constexpr BitInt& operator=(const OtherType other) noexcept
    {
        value = truncateToReal(other);

        return *this;
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>  // NOLINTNEXTLINE(*-explicit-constructor)
    constexpr BitInt(const BitInt<other_bits>& other) noexcept : value(truncateToReal(other))
    {
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>
    constexpr BitInt& operator=(const BitInt<other_bits>& other) noexcept
    {
        if (static_cast<void*>(this) == static_cast<const void*>(&other)) {
            return *this;
        }

        value = truncateToReal(static_cast<SignedType>(other));

        return *this;
    }

    // ---------------------------- 一元 ---------------------------------
    // =========================== ~

    constexpr SignedType operator~() const noexcept
    {
        const SignedType tmp = transToSigned(value);

        return ~tmp;
    }

    // =========================== !

    constexpr bool operator!() const noexcept
    {
        return !static_cast<bool>(transToSigned(value));
    }

    // =========================== +/-

    constexpr SignedType operator+() const noexcept
    {
        return SignedType(*this);
    }
    constexpr SignedType operator-() const noexcept
    {
        return -transToSigned(value);
    }

    // ---------------------------- 二元 ---------------------------------
    // =========================== +

    template<typename OtherType>
    constexpr SignedType operator+(const OtherType& other) const noexcept
    {
        return transToSigned(this->value) + static_cast<SignedType>(other);
    }

    template<ArithmeticType OtherType>
    friend constexpr SignedType operator+(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return static_cast<SignedType>(lhs) + transToSigned(rhs.value);
    }

    // =========================== -

    template<typename OtherType>
    constexpr SignedType operator-(const OtherType& other) const noexcept
    {
        return transToSigned(this->value) - static_cast<SignedType>(other);
    }

    template<ArithmeticType OtherType>
    friend constexpr SignedType operator-(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return (static_cast<SignedType>(lhs) - transToSigned(rhs.value));
    }

    // =========================== *

    template<typename OtherType>
    constexpr SignedType operator*(const OtherType& other) const noexcept
    {
        return (transToSigned(this->value) * static_cast<SignedType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr SignedType operator*(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return (static_cast<SignedType>(lhs) * transToSigned(rhs.value));
    }

    // =========================== /

    template<typename OtherType>
    constexpr SignedType operator/(const OtherType& other) const EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
        const auto other_signed = static_cast<SignedType>(other);

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        if (other_signed == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
#endif

        return (transToSigned(this->value) / other_signed);
    }

    template<ArithmeticType OtherType>
    friend constexpr SignedType operator/(const OtherType& lhs, const BitInt& rhs) EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
        const auto rhs_signed = transToSigned(rhs.value);

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        if (rhs_signed == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
#endif

        return (static_cast<SignedType>(lhs) / rhs_signed);
    }

    // =========================== %

    template<typename OtherType>
    constexpr SignedType operator%(const OtherType& other) const EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
        const auto other_signed = static_cast<SignedType>(other);

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        if (other_signed == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
#endif

        return (transToSigned(this->value) % other_signed);
    }

    template<ArithmeticType OtherType>
    friend constexpr SignedType operator%(const OtherType& lhs, const BitInt& rhs) EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
        const auto rhs_signed = transToSigned(rhs.value);

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        if (rhs_signed == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
#endif

        return (static_cast<SignedType>(lhs) % rhs_signed);
    }

    // =========================== &

    template<typename OtherType>
    constexpr SignedType operator&(const OtherType& other) const noexcept
    {
        return (transToSigned(this->value) & static_cast<SignedType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr SignedType operator&(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return (static_cast<SignedType>(lhs) & transToSigned(rhs.value));
    }

    // =========================== |

    template<typename OtherType>
    constexpr SignedType operator|(const OtherType& other) const noexcept
    {
        return (transToSigned(this->value) | static_cast<SignedType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr SignedType operator|(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return (static_cast<SignedType>(lhs) | transToSigned(rhs.value));
    }

    // =========================== ^

    template<typename OtherType>
    constexpr SignedType operator^(const OtherType& other) const noexcept
    {
        return (transToSigned(this->value) ^ static_cast<SignedType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr SignedType operator^(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return (static_cast<SignedType>(lhs) ^ transToSigned(rhs.value));
    }

    // =========================== <<

    template<std::integral OtherType>
    constexpr SignedType operator<<(const OtherType shift) const noexcept
    {
        return (transToSigned(value) << shift);
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>
    constexpr SignedType operator<<(const BitInt<other_bits> shift) const noexcept
    {
        return (transToSigned(value) << SignedType(shift));
    }

    // =========================== >>

    template<std::integral OtherType>
    constexpr SignedType operator>>(const OtherType shift) const noexcept
    {
        return (transToSigned(value) >> shift);
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>
    constexpr SignedType operator>>(const BitInt<other_bits> shift) const noexcept
    {
        return (transToSigned(value) >> SignedType(shift));
    }

    // ---------------------------- 复合赋值 -------------------------------
    // =========================== +=

    template<typename OtherType>
    constexpr BitInt& operator+=(const OtherType& other) noexcept
    {
        return *this = *this + other;
    }

    // =========================== -=

    template<typename OtherType>
    constexpr BitInt& operator-=(const OtherType& other) noexcept
    {
        return *this = *this - other;
    }

    // =========================== *=

    template<typename OtherType>
    constexpr BitInt& operator*=(const OtherType& other) noexcept
    {
        return *this = *this * other;
    }

    // =========================== /=

    template<typename OtherType>
    constexpr BitInt& operator/=(const OtherType& other) EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
        const auto other_signed = static_cast<SignedType>(other);

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        if (other_signed == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
#endif

        return *this = *this / other_signed;
    }

    // =========================== %=

    template<typename OtherType>
    constexpr BitInt& operator%=(const OtherType& other) EMDEVIF_UTIL_BITINT_NOEXCEPT
    {
        const auto other_signed = static_cast<SignedType>(other);

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
        if (other_signed == 0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
#endif

        return *this = *this % other_signed;
    }

    // =========================== &=

    template<typename OtherType>
    constexpr BitInt& operator&=(const OtherType& other) noexcept
    {
        return *this = *this & other;
    }

    // =========================== |=

    template<typename OtherType>
    constexpr BitInt& operator|=(const OtherType& other) noexcept
    {
        return *this = *this | other;
    }

    // =========================== ^=

    template<typename OtherType>
    constexpr BitInt& operator^=(const OtherType& other) noexcept
    {
        return *this = *this ^ other;
    }

    // =========================== <<=

    template<std::integral OtherType>
    constexpr BitInt& operator<<=(const OtherType shift) const noexcept
    {
        return *this = *this << shift;
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>
    constexpr BitInt& operator<<=(const BitInt<other_bits> shift) const noexcept
    {
        return *this = *this << shift;
    }

    // =========================== >>=

    template<std::integral OtherType>
    constexpr BitInt& operator>>=(const OtherType shift) const noexcept
    {
        return *this = *this >> shift;
    }

    template<BitsType_t other_bits>
        requires ValidBitIntWidth<other_bits>
    constexpr BitInt& operator>>=(const BitInt<other_bits> shift) const noexcept
    {
        return *this = *this >> shift;
    }

    // ---------------------------- 自增/自减 ----------------------------
    // =========================== ++

    constexpr SignedType operator++() noexcept
    {
        return *this += 1;
    }
    constexpr SignedType operator++(int) noexcept
    {
        const auto tmp = *this;
        ++(*this);
        return tmp;
    }

    // =========================== --

    constexpr SignedType operator--() noexcept
    {
        return *this -= 1;
    }
    constexpr SignedType operator--(int) noexcept
    {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    // ---------------------------- 比较 ----------------------------
    // =========================== ==

    template<typename OtherType>
    constexpr bool operator==(const OtherType& other) const noexcept
    {
        return (transToSigned(value) == static_cast<SignedType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator==(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return (static_cast<SignedType>(lhs) == transToSigned(rhs.value));
    }

    // =========================== !=

    template<typename OtherType>
    constexpr bool operator!=(const OtherType& other) const noexcept
    {
        return (transToSigned(value) != static_cast<SignedType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator!=(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return (static_cast<SignedType>(lhs) != transToSigned(rhs.value));
    }

    // =========================== >

    template<typename OtherType>
    constexpr bool operator>(const OtherType& other) const noexcept
    {
        return (transToSigned(value) > static_cast<SignedType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator>(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return (static_cast<SignedType>(lhs) > transToSigned(rhs.value));
    }

    // =========================== <

    template<typename OtherType>
    constexpr bool operator<(const OtherType& other) const noexcept
    {
        return (transToSigned(value) < static_cast<SignedType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator<(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return (static_cast<SignedType>(lhs) < transToSigned(rhs.value));
    }

    // =========================== >=

    template<typename OtherType>
    constexpr bool operator>=(const OtherType& other) const noexcept
    {
        return (transToSigned(value) >= static_cast<SignedType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator>=(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return (static_cast<SignedType>(lhs) >= transToSigned(rhs.value));
    }

    // =========================== <=

    template<typename OtherType>
    constexpr bool operator<=(const OtherType& other) const noexcept
    {
        return (transToSigned(value) <= static_cast<SignedType>(other));
    }

    template<ArithmeticType OtherType>
    friend constexpr bool operator<=(const OtherType& lhs, const BitInt& rhs) noexcept
    {
        return (static_cast<SignedType>(lhs) <= transToSigned(rhs.value));
    }

    // --------------------------- stream ------------------------------

    inline friend std::ostream& operator<<(std::ostream& os, const BitInt& bit_int)
    {
        os << static_cast<SignedType>(bit_int);
        return os;
    }

    // end operators ---------------------------------------------------

    template<std::integral OtherType>
    operator OtherType() const noexcept  // NOLINT(*-explicit-constructor)
    {
        return static_cast<OtherType>(transToSigned(value));
    }

    ~BitInt() noexcept = default;

private:
    RealType value;
};

template<>
class BitInt<0>
{
public:
    static constexpr int8_t max() noexcept
    {
        return 0;
    }

    static constexpr int8_t min() noexcept
    {
        return 0;
    }

    inline friend std::ostream& operator<<(std::ostream& os, const BitInt&)
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
    const int8_t value{0};
};

}  // namespace emdevif
