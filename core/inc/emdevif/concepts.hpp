/**
 * @file concepts.hpp
 * @author 杜以成
 * @date 2025-06-07
 * @brief 概念定义
 */

#pragma once
#ifndef EMDEVIF_CONCEPTS_HPP
#define EMDEVIF_CONCEPTS_HPP

#include <type_traits>

namespace emdevif {

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
 * 矩阵可以相乘的概念
 * @tparam row1 乘号左边的矩阵的行数
 * @tparam col1 乘号左边的矩阵的列数
 * @tparam row2 乘号右边的矩阵的行数
 * @tparam col2 乘号右边的矩阵的列数
 */
template<std::size_t row1, std::size_t col1, std::size_t row2, std::size_t col2>
concept MatrixCouldMultiplied = (col1 == row2);

/**
 * 方阵概念
 * @tparam row 矩阵的行数
 * @tparam col 矩阵的列数
 */
template<std::size_t row, std::size_t col>
concept SquareMatrix = (row == col);

}  // namespace emdevif

#endif  // !EMDEVIF_CONCEPTS_HPP
