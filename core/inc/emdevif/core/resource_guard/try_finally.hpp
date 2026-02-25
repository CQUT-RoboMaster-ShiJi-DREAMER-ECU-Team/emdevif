/**
 * @file try_finally.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_CORE_RESOURCE_GUARD_TRY_FINALLY_HPP
#define EMDEVIF_CORE_RESOURCE_GUARD_TRY_FINALLY_HPP

#include "emdevif/core/detail/config.hpp"

EMDEVIF_MODULE_EXPORT
namespace emdevif {

template<typename R, typename TryBlock, typename FinallyBlock>
constexpr R tryFinally(const TryBlock& try_block, const FinallyBlock& finally_block) noexcept
{
    R result{try_block()};
    finally_block();
    return result;
}

template<typename TryBlock, typename FinallyBlock>
constexpr void tryFinally(const TryBlock& try_block, const FinallyBlock& finally_block) noexcept
{
    try_block();
    finally_block();
}

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_RESOURCE_GUARD_TRY_FINALLY_HPP
