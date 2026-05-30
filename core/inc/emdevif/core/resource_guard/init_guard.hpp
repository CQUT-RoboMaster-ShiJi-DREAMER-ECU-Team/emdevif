/**
 * @file init_guard.hpp
 * @brief RAII 风格的 init/deInit 自动管理
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

/**
 * @brief 拥有 init / deInit 配对接口的对象概念
 * @tparam T 对象类型
 * @tparam InitArgs init 参数类型
 */
template<typename T, typename... InitArgs>
concept ValidHaveInitDeInitPairObject = requires(T obj, InitArgs&&... init_args) {
    { obj.init(std::forward<InitArgs>(init_args)...) } -> std::same_as<ErrorCode>;
    { obj.deInit() } -> std::same_as<void>;
};

/**
 * @brief 初始化守卫标记：不自动初始化对象
 */
enum class InitGuardDoNotInitObjectTag {
    value_
};

/// 初始化守卫标记常量（不自动初始化对象）
constexpr auto init_guard_do_not_init_object_tag = InitGuardDoNotInitObjectTag::value_;

/**
 * @brief RAII 风格的 init/deInit 自动管理守卫
 * @tparam T 托管的对象类型
 */
template<typename T>
class InitGuard
{
public:
    using ValueType = T;

private:
    ValueType& object_;  ///< 托管的对象的引用

public:
    InitGuard() = delete;

    /**
     * 构造 InitGuard 并自动调用对象的 init 函数
     * @attention 如果是在关闭了异常的环境下，使用该构造函数会导致编译失败。可以通过调用另一个重载然后手动执行初始化。
     * @tparam InitArgs init 参数类型
     * @param object 托管的对象
     * @param init_args 传递给 init 的参数
     * @throws ErrorWithCodeException 如果 init 失败
     */
    template<typename... InitArgs>
        requires ValidHaveInitDeInitPairObject<ValueType, InitArgs...>
    explicit InitGuard(ValueType& object, InitArgs&&... init_args) : object_(object)
    {
        if (const ErrorCode ec = object_.init(std::forward<InitArgs>(init_args)...); ec != ErrorCode::Success) {
            throw ErrorWithCodeException(ec, "Failed to initialize object in InitGuard.");
        }
    }
    /**
     * @overload
     * 构造 InitGuard 但不自动调用 init
     * @note 一般需要通过 InitGuard 实例的 init 方法来手动初始化被托管对象
     * @param InitGuardDoNotInitObjectTag 不初始化标记
     * @param object 托管的对象
     */
    explicit InitGuard(InitGuardDoNotInitObjectTag, ValueType& object) noexcept : object_(object) {}

    ~InitGuard() noexcept
    {
        object_.deInit();
    }

public:
    /**
     * 调用被托管的对象的 init 方法
     * @note 一般在使用了接受 init_guard_do_not_init_object_tag 的构造函数后手动调用
     * @tparam InitArgs init 参数类型
     * @param init_args 传递给 init 的参数
     * @return 错误码
     */
    template<typename... InitArgs>
        requires ValidHaveInitDeInitPairObject<ValueType, InitArgs...>
    ErrorCode init(InitArgs&&... init_args) noexcept
    {
        return object_.init(std::forward<InitArgs>(init_args)...);
    }
};

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_RESOURCE_GUARD_INIT_GUARD_HPP
