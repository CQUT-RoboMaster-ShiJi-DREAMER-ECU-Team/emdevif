/**
 * @file fatal_handler.hpp
 * @brief 致命错误处理
 */

#pragma once
#ifndef EMDEVIF_CORE_ERROR_HANDLER_FATAL_HANDLER_HPP
#define EMDEVIF_CORE_ERROR_HANDLER_FATAL_HANDLER_HPP

#include "emdevif/core/detail/config.hpp"

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
#include "emdevif/core/attributes_and_useful_macros.h"

#include <cstdarg>

#include <type_traits>
#endif

namespace emdevif {

/**
 * 终止程序的函数指针类型
 */
EMDEVIF_MODULE_EXPORT using TerminateFunction = void(*)();

/**
 * 终止整个程序
 */
EMDEVIF_MODULE_EXPORT EMDEVIF_NO_RETURN void terminate();

/**
 * 注册终止函数（不具备线程安全性，建议只在初始化的时候调用）。
 * 如果不调用这个函数，那么将调用默认的终止函数。
 * @param func 终止函数。注意该函数不能返回，否则是未定义行为。
 */
EMDEVIF_MODULE_EXPORT void registerTerminateFunction(TerminateFunction func) noexcept;

/**
 * 致命错误回调函数指针
 * @param file 将会传入致命错误产生的文件名称
 * @param line 将会传入致命错误产生的行号
 * @param format 将会传入消息的格式化字符串
 * @param args 将会传入填充占位符的可变参列表
 */
EMDEVIF_MODULE_EXPORT using FatalHandlerCallBack =
    void(*)(const char* file, int line, const char* format, std::va_list args);

/**
 * 致命错误处理函数
 * @attention 不应当直接使用这个函数，应该使用宏 EMDEVIF_FATAL_HANDLER，
 * 可以自动填充文件名与行号，但需要引入头文件 @ref emdevif/core/fatal_handler.hpp
 * @param file 所在文件名
 * @param line 所在行号
 * @param format 错误信息的格式化字符串
 * @param args 填充占位符的可变参列表
 */
EMDEVIF_MODULE_EXPORT EMDEVIF_NO_RETURN void fatalHandler(const char* file,
                                                          int line,
                                                          const char* format,
                                                          std::va_list args) noexcept;

/**
 * @overload
 * @param ... 填充占位符的值
 */
EMDEVIF_MODULE_EXPORT EMDEVIF_NO_RETURN void fatalHandler(const char* file, int line, const char* format, ...) noexcept
    EMDEVIF_FORMAT_CHECK(printf, 3, 4);

/**
 * 注册致命错误回调函数（不具备线程安全性，建议只在初始化的时候调用）。
 * 如果不调用这个函数，那么将不会有额外的处理，直接调用 terminate。
 * @param callback 回调函数
 */
EMDEVIF_MODULE_EXPORT void registerFatalHandler(FatalHandlerCallBack callback) noexcept;

/**
 * 断言失败处理函数指针类型
 */
EMDEVIF_MODULE_EXPORT using AssertFailedHandler =
    void(*)(const char* file, int line, const char* func_name, const char* condition_name, const char* message);

namespace detail {

/**
 * 断言失败处理函数指针（编译期初始化为默认的断言失败处理函数）
 */
extern AssertFailedHandler assertFailedHandler;

/// 用于在编译期触发断言失败的函数，详见 @ref emdevif_assert 函数内部的注释
void illFormatedCodeToAssertFailed() noexcept;

}  // namespace detail

/**
 * 断言入口
 * @attention 不应直接调用这个函数，应当使用宏 @ref EMDEVIF_ASSERT，可以自动填充文件名、行号、函数名与表达式名称，
 * 但需要引入头文件 @ref emdevif/core/fatal_handler.hpp
 * @note 为了避免与 <cassert> 中的 assert 冲突，故将此函数命名为 emdevif_assert
 *
 * @param condition 待断言的表达式
 * @param file 调用函数所在文件
 * @param line 调用函数所在行号
 * @param func_name 包含函数名称
 * @param condition_name 表达式名称
 * @param message 信息
 */
EMDEVIF_MODULE_EXPORT constexpr void emdevif_assert(const bool condition,
                                                    const char* file,
                                                    const int line,
                                                    const char* func_name,
                                                    const char* condition_name,
                                                    const char* message = "") noexcept
{
    // 把这个函数设置为 constexpr 是为了在常量表达式函数里使用（运行时也可以断言）

    if (!condition) {
        if (std::is_constant_evaluated()) {
            // 在常量表达式求值上下文中，用 illFormatedCodeToAssertFailed 函数触发编译错误表示断言失败。
            //
            // 如果在编译时提示“在常量求值上下文调用了非 constexpr 的 illFormatedCodeToAssertFailed 函数”，
            // 说明是触发了断言失败，您可以查看编译错误信息提示的调用栈查看触发断言的位置。
            detail::illFormatedCodeToAssertFailed();
            return;
        }

        if (detail::assertFailedHandler != nullptr) {
            detail::assertFailedHandler(file, line, func_name, condition_name, message);
        }

        terminate();
    }
}

/**
 * 断言失败处理函数注册（不具备线程安全性，建议只在初始化的时候调用）。
 * 如果不调用这个函数，那么将调用默认的断言失败处理函数。
 * @param handler 回调函数
 */
EMDEVIF_MODULE_EXPORT void registerAssertFailedHandler(AssertFailedHandler handler) noexcept;

}  // namespace emdevif

#endif  // !EMDEVIF_CORE_ERROR_HANDLER_FATAL_HANDLER_HPP
