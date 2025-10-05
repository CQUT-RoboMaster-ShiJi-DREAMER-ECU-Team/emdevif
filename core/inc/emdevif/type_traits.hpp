/**
 * @file type_traits.hpp
 * @author DuYicheng
 * @date 2025-10-05
 * @brief 类型特征（STL type_traits 头文件的扩展）
 */

#pragma once
#ifndef EMDEVIF_TYPE_TRAITS_HPP
#define EMDEVIF_TYPE_TRAITS_HPP

#include <type_traits>

namespace emdevif {

// 参考 https://github.com/microsoft/proxy/issues/105
template<typename Expr>
consteval bool is_consteval(Expr)
{
    return requires { typename std::bool_constant<(Expr{}(), false)>; };
}

}  // namespace emdevif

#endif  // !EMDEVIF_TYPE_TRAITS_HPP
