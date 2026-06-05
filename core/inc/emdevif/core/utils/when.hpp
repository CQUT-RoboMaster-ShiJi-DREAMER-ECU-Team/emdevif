/**
 * @file when.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_CORE_UTILS_WHEN_HPP
    #define EMDEVIF_CORE_UTILS_WHEN_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include <utility>
        #include <type_traits>
        #include <concepts>
    #endif

namespace emdevif {

EMDEVIF_MODULE_EXPORT_BEGIN

/// 默认标签类型
struct default_tag_t {
};

/// 默认标签，用于 when 表达式里
inline constexpr default_tag_t default_tag{};

EMDEVIF_MODULE_EXPORT_END

namespace detail {

template<typename T>
struct is_default_tag : std::false_type {
};
template<>
struct is_default_tag<default_tag_t> : std::true_type {
};

template<typename T>
inline constexpr bool is_default_tag_v = is_default_tag<T>::value;

// 递归终止：无匹配也无 default
template<typename R, typename T>
constexpr R whenImpl(const T&)
{
    return R();
}

// 主递归：匹配 v == case_val，执行 func
template<typename R, typename T, typename CaseVal, typename Func, typename... Rest>
constexpr R whenImpl(const T& v, CaseVal&& case_val, Func&& func, Rest&&... rest)
{
    if constexpr (is_default_tag_v<std::remove_cvref_t<CaseVal>>) {
        return std::forward<Func>(func)();
    }
    else {
        if (v == std::forward<CaseVal>(case_val)) {
            return std::forward<Func>(func)();
        }
        else {
            return whenImpl<R>(v, std::forward<Rest>(rest)...);
        }
    }
}

template<typename... Args>
struct WhenResult {
    using type = void;
};

template<typename CaseVal1, typename Func1, typename... Rest>
struct WhenResult<CaseVal1, Func1, Rest...> : std::invoke_result<Func1> {
};

template<typename... Args>
using WhenResult_t = typename WhenResult<Args...>::type;

// clang-format off
template<typename R, typename T, typename CaseVal, typename Func, typename... Rest>
struct IsWhenArgs : std::conjunction<
    std::negation< is_default_tag<std::remove_cvref_t<CaseVal>> >,
    std::is_invocable<Func>,
    std::is_same<R, std::invoke_result_t<Func>>,
    std::bool_constant<
        requires(const T ct, const CaseVal cv) {
            { ct == cv } noexcept -> std::convertible_to<bool>;
        }
    >,
    IsWhenArgs<R, T, Rest...>
> {
};
// clang-format on

template<typename R, typename T, typename CaseVal, typename Func>
struct IsWhenArgs<R, T, CaseVal, Func> : std::true_type {
};

template<typename R, typename T, typename... Args>
inline constexpr bool is_when_args_v = IsWhenArgs<R, T, Args...>::value;

template<typename CaseVal, typename Func, typename... Rest>
struct IsNothrowWhenArgs : std::conjunction<std::is_nothrow_invocable<Func>, IsNothrowWhenArgs<Rest...>> {
};

template<typename CaseVal, typename Func>
struct IsNothrowWhenArgs<CaseVal, Func> : std::true_type {
};

template<typename... Args>
inline constexpr bool is_nothrow_when_args_v = IsNothrowWhenArgs<Args...>::value;

}  // namespace detail

/**
 * when 表达式：更加通用的 switch 表达式
 *
 * 对于 switch 表达式：
 * @code{cpp}
 * switch (value) {
 * case 1: doSomething1(); break;
 * case 2: doSomething2(); break;
 * default: doDefault(); break;
 * }
 * @endcode
 * 它等效于：
 * @code{cpp}
 * emdevif::when(value,
 *     1, [&] { doSomething1(); },
 *     2, [&] { doSomething2(); },
 *     emdevif::default_tag, [&]{ doDefault(); }
 * );
 * @endcode
 */
EMDEVIF_MODULE_EXPORT
template<typename T, typename... Args>
constexpr decltype(auto) when(const T& v, Args&&... args) noexcept(detail::is_nothrow_when_args_v<Args...>)
{
    static_assert(sizeof...(Args) % 2 == 0, "Arguments should be in pairs of (target, func) or (default_tag, func)");

    using ResultType = detail::WhenResult_t<Args...>;
    static_assert(detail::is_when_args_v<ResultType, T, Args...>,
                  "All cases value should be comparable with 'equals'(operator ==) without throw any exceptions, and "
                  "all functions arguments should be invocable with no parameters. Pay attention to that, the "
                  "`default_tag`-`default_func` must be the last argument pair if use default explicitly");

    return detail::whenImpl<ResultType>(v, std::forward<Args>(args)...);
}

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_UTILS_WHEN_HPP
