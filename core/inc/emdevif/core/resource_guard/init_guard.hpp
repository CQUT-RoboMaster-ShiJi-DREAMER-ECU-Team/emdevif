/**
 * @file init_guard.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_CORE_RESOURCE_GUARD_INIT_GUARD_HPP
#define EMDEVIF_CORE_RESOURCE_GUARD_INIT_GUARD_HPP

#include "emdevif/core/detail/config.hpp"

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#include "emdevif/core/error_handler.hpp"

#include <utility>
#include <concepts>
#endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

template<typename T, typename... InitArgs>
concept ValidHaveInitDeInitPairObject = requires(T obj, InitArgs&&... init_args) {
    { obj.init(std::forward<InitArgs>(init_args)...) } -> std::same_as<ErrorCode>;
    { obj.deInit() } -> std::same_as<void>;
};

enum class InitGuardDoNotInitObjectTag {
    value_
};

constexpr auto init_guard_do_not_init_object_tag = InitGuardDoNotInitObjectTag::value_;

template<typename T>
class InitGuard
{
public:
    using ValueType = T;

private:
    ValueType& object_;

public:
    InitGuard() = delete;

    template<typename... InitArgs>
        requires ValidHaveInitDeInitPairObject<ValueType, InitArgs...>
    explicit InitGuard(ValueType& object, InitArgs&&... init_args) : object_(object)
    {
        if (const ErrorCode ec = object_.init(std::forward<InitArgs>(init_args)...); ec != ErrorCode::Success) {
            throw ErrorWithCodeException(ec, "Failed to initialize object in InitGuard.");
        }
    }

    explicit InitGuard(InitGuardDoNotInitObjectTag, ValueType& object) noexcept : object_(object) {}

    ~InitGuard() noexcept
    {
        object_.deInit();
    }

public:
    template<typename... InitArgs>
        requires ValidHaveInitDeInitPairObject<ValueType, InitArgs...>
    ErrorCode init(InitArgs&&... init_args) noexcept
    {
        return object_.init(std::forward<InitArgs>(init_args)...);
    }
};

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_RESOURCE_GUARD_INIT_GUARD_HPP
