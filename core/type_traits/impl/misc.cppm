/**
 * @file misc.cppm
 * @module emdevif.typeTraits:misc
 * @author DuYicheng
 * @date 2025-10-08
 * @brief 类型特征 - 杂项
 */

module;

#include <type_traits>

export module emdevif.typeTraits:misc;

export namespace emdevif {

// 参考 https://github.com/microsoft/proxy/issues/105
template<typename Expr>
consteval bool is_consteval(Expr)
{
    return requires { typename std::bool_constant<(Expr{}(), false)>; };
}

}  // namespace emdevif
