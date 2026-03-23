/**
 * @file concepts.hpp
 * @brief 概念
 */

#pragma once
#ifndef EMDEVIF_CORE_CONCEPTS_HPP
    #define EMDEVIF_CORE_CONCEPTS_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include <type_traits>
        #include <concepts>
        #include <compare>
        #include <limits>
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

/**
 * 有界的类型概念
 */
template<typename T>
concept HaveLimitType = requires {
    std::numeric_limits<T>::max();
    std::numeric_limits<T>::min();
} || requires {
    T::max();
    T::min();
} || requires(T v, const T cv) {
    v.max();
    v.min();

    cv.max();
    cv.min();
};

/**
 * 可计算类型概念
 * @tparam Type 数据类型
 */
template<typename Type>
concept ArithmeticType = requires(Type lhs, Type rhs) {
    lhs + rhs;
    lhs - rhs;
    /* clang-format off */
    lhs * rhs;
    /* clang-format on */
    lhs / rhs;
    lhs = rhs;
    lhs <=> rhs;
};

/**
 * 浮点类型概念
 * @tparam Type 数据类型
 */
template<typename Type>
concept FloatingPointType = std::is_floating_point_v<Type>;

/**
 * 指针类型概念
 * @tparam Type 类型
 */
template<typename Type>
concept PointerType = std::is_pointer_v<Type>;

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_CONCEPTS_HPP
