/**
 * @file defer.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_CORE_RESOURCE_GUARD_DEFER_HPP
    #define EMDEVIF_CORE_RESOURCE_GUARD_DEFER_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include <utility>
        #include <concepts>
        #include <functional>
    #endif

    #include "emdevif/core/simplify_decl_macros.hpp"

EMDEVIF_MODULE_EXPORT
namespace emdevif {

template<typename Func>
concept DeferFunction = std::is_nothrow_invocable_r_v<void, Func>;

template<DeferFunction Func>
class Defer final
{
public:
    using FunctionType = Func;

private:
    FunctionType func_;

public:
    explicit constexpr Defer(const FunctionType& func) : func_(func) {}
    explicit constexpr Defer(FunctionType&& func) : func_(std::move(func)) {}

    EMDEVIF_DELETE_COPY_AND_MOVE(Defer);

    constexpr ~Defer() noexcept
    {
        std::invoke(func_);
    }
};

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_RESOURCE_GUARD_DEFER_HPP
