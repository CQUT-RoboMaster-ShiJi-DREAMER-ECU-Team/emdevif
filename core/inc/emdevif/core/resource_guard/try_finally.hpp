/**
 * @file try_finally.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_CORE_RESOURCE_GUARD_TRY_FINALLY_HPP
    #define EMDEVIF_CORE_RESOURCE_GUARD_TRY_FINALLY_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include <utility>
        #include <concepts>
        #include <functional>
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

template<typename Func>
concept ValidTryFunction = std::is_invocable_v<Func>;

template<typename Func>
concept ValidFinallyFunction = std::is_nothrow_invocable_r_v<void, Func>;

template<ValidTryFunction TryFunc, ValidFinallyFunction FinallyFunc>
constexpr auto tryFinally(TryFunc&& try_block, FinallyFunc&& finally) -> std::invoke_result_t<TryFunc>
{
    [[maybe_unused]] Defer _{[&] noexcept { std::invoke(std::forward<FinallyFunc>(finally)); }};
    return std::invoke(std::forward<TryFunc>(try_block));
}

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_RESOURCE_GUARD_TRY_FINALLY_HPP
