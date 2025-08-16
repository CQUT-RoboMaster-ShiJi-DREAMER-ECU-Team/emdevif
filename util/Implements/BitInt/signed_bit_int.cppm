/**
 * @file signed_bit_int.cppm
 * @module emdevif.util.BitInt:signed_partial
 * @author DuYicheng
 * @date 2025-08-16
 * @brief 有符号指定宽度整型
 */

// ReSharper disable CppNonExplicitConversionOperator

module;

#include <cstdint>

#include <concepts>

export module emdevif.util.BitInt:signed_partial;

namespace emdevif {

export using BitsType_t = uint_fast8_t;

export template<BitsType_t bits>
concept ValidBitIntWidth = (bits <= 64U);

export template<std::integral T>
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

export constexpr BitsType_t bitsSetValue(const BitsType_t bits) noexcept
{
    BitsType_t result = 0U;

    for (BitsType_t i = 0U; i < bits; ++i) {
        result |= (1U << i);
    }

    return result;
}

template<typename Type, BitsType_t bits>
concept ValidRealType = bitsOf<Type>() <= bits;

template<std::signed_integral RealType, BitsType_t bits>
    requires ValidBitIntWidth<bits> && ValidRealType<RealType, bits>
class BitIntImpl
{
private:
    consteval void cutHigherBits() noexcept
    {
        constexpr auto real_type_bits = bitsOf<RealType>();

        if constexpr (bits < real_type_bits) {
            value &= bitsSetValue(bits);
        }
    }

public:
    constexpr BitIntImpl() : value() {}

    template<std::integral OtherType>
    constexpr BitIntImpl(const OtherType other) : value(other)  // NOLINT(*-explicit-constructor)
    {
        cutHigherBits();
    }

    template<std::integral OtherType>
    constexpr BitIntImpl& operator=(const OtherType other)
    {
        this->value = other;
        cutHigherBits();

        return *this;
    }

    // constexpr BitIntImpl(const BitIntImpl& other) : value(other.value) {}
    //
    // constexpr BitIntImpl& operator=(const BitIntImpl& other)
    // {
    //     if (this == &other) {
    //         return *this;
    //     }
    //
    //     this->value = other.value;
    //
    //     return *this;
    // }

    template<std::integral OtherType>
    operator OtherType() const noexcept  // NOLINT(*-explicit-constructor)
    {
        return static_cast<OtherType>(value);
    }

    ~BitIntImpl() = default;

protected:
    RealType value;
};

}  // namespace emdevif

export namespace emdevif {

template<BitsType_t bits, typename sfinae>
    requires ValidBitIntWidth<bits>
class BitInt;

template<>
class BitInt<0, void>
{
public:
    template<std::integral Type>
    operator Type() const noexcept  // NOLINT(*-explicit-constructor)
    {
        return 0;
    }
};

template<BitsType_t bits>
    requires ValidBitIntWidth<bits>
class BitInt<bits, std::enable_if_t<bits <= 8U, void>> : public BitIntImpl<int8_t, bits>
{
};

}  // namespace emdevif
