/**
 * @file when.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_CORE_WHEN_HPP
    #define EMDEVIF_CORE_WHEN_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include <utility>
        #include <type_traits>
        #include <concepts>
    #endif

namespace emdevif {

// 默认标签类型
struct default_tag_t {
};
inline constexpr default_tag_t default_tag{};

namespace detail {

// 辅助：检查是否是 default_tag
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
            return whenImpl(v, std::forward<Rest>(rest)...);
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
template<typename R, typename CaseVal, typename Func, typename... Rest>
struct IsWhenArgs : std::conjunction<
    std::is_invocable<Func>,
    std::is_same<R, std::invoke_result_t<Func>>,
    std::bool_constant<
        requires(const CaseVal cv) {
            { cv == cv } noexcept -> std::convertible_to<bool>;
        }
    >,
    IsWhenArgs<R, Rest...>
> {
};
// clang-format on

template<typename R, typename CaseVal, typename Func>
struct IsWhenArgs<R, CaseVal, Func> : std::true_type {
};

template<typename R, typename... Args>
inline constexpr bool is_when_args_v = IsWhenArgs<R, Args...>::value;

template<typename CaseVal, typename Func, typename... Rest>
struct IsNothrowWhenArgs : std::conjunction<std::is_nothrow_invocable<Func>, IsNothrowWhenArgs<Rest...>> {
};

template<typename CaseVal, typename Func>
struct IsNothrowWhenArgs<CaseVal, Func> : std::true_type {
};

template<typename... Args>
inline constexpr bool is_nothrow_when_args_v = IsNothrowWhenArgs<Args...>::value;

}  // namespace detail

EMDEVIF_MODULE_EXPORT
template<typename T, typename... Args>
constexpr decltype(auto) when(const T& v, Args&&... args) noexcept(detail::is_nothrow_when_args_v<Args...>)
{
    static_assert(sizeof...(Args) % 2 == 0, "Arguments should be in pairs of (target, func) or (default_tag, func)");

    using ResultType = detail::WhenResult_t<Args...>;
    static_assert(detail::is_when_args_v<ResultType, Args...>,
                  "All cases value should be comparable with 'equals'(operator ==) without throw any exceptions, and "
                  "all functions arguments should be invocable with no parameters");

    return detail::whenImpl<ResultType>(v, std::forward<Args>(args)...);
}

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_WHEN_HPP
