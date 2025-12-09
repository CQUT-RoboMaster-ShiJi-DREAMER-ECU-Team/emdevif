/**
 * @file concepts.cppm
 * @brief 概念
 */

module;

#include <type_traits>

export module emdevif.concepts;

export namespace emdevif {

/**
 * 可计算类型概念
 * @tparam Type 数据类型
 */
template<typename Type>
concept ArithmeticType = std::is_arithmetic_v<Type>;

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
