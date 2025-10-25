/**
 * @file fatal_handler.cppm
 * @author DuYicheng
 * @date 2025-09-05
 * @brief 致命错误处理函数与断言
 */

module;

#include <cstdarg>

#include "emdevif/attributes_and_useful_macros.h"

export module emdevif.errorHandler:fatalHandlerAndAssert;

namespace emdevif {

/**
 * 默认的程序终止函数
 */
EMDEVIF_NO_RETURN void defaultTerminateFunction() noexcept;

/**
 * 终止程序的函数指针类型
 */
export using TerminateFunction = void(*)();

/**
 * 终止程序的函数指针（编译期初始化为默认的终止函数）
 */
constinit TerminateFunction terminateFunction = defaultTerminateFunction;

/**
 * 终止整个程序
 */
export EMDEVIF_NO_RETURN void terminate()
{
    terminateFunction();

    // 正常情况下，程序不应该运行到这里。但可能出现用户实现的 terminateFunction 返回的情况，
    // 因此增加进入死循环以确保不会返回，同时避免编译器警告。
    while (true) {
    }
}

/**
 * 注册终止函数（不具备线程安全性，建议只在初始化的时候调用）。
 * 如果不调用这个函数，那么将调用默认的终止函数。
 * @param func 终止函数。注意该函数不能返回，否则是未定义行为。
 */
export void registerTerminateFunction(TerminateFunction func) noexcept;

/**
 * 致命错误回调函数指针
 * @param file 将会传入致命错误产生的文件名称
 * @param line 将会传入致命错误产生的行号
 * @param format 将会传入消息的格式化字符串
 * @param args 将会传入填充占位符的可变参列表
 */
export using FatalHandlerCallBack = void(*)(const char* file, int line, const char* format, std::va_list args);

/**
 * 致命错误回调函数（编译期初始化为默认的回调函数）
 */
constinit FatalHandlerCallBack fatalHandlerCallback = nullptr;
// 默认为 nullptr，表示不进行额外处理，直接调用 terminate

/**
 * 致命错误处理函数
 * @attention 不应当直接使用这个函数，应该使用宏 EMDEVIF_FATAL_HANDLER，
 * 可以自动填充文件名与行号，但需要引入头文件 @ref emdevif/fatal_handler.hpp
 * @param file 所在文件名
 * @param line 所在行号
 * @param format 错误信息的格式化字符串
 * @param args 填充占位符的可变参列表
 */
export EMDEVIF_NO_RETURN void fatalHandler(const char* file,
                                           const int line,
                                           const char* format,
                                           std::va_list args) noexcept
{
    if (fatalHandlerCallback != nullptr) {
        fatalHandlerCallback(file, line, format, args);
    }

    terminate();
}
/**
 * @overload
 * @param ... 填充占位符的值
 */
export EMDEVIF_NO_RETURN void fatalHandler(const char* file, const int line, const char* format, ...) noexcept
{
    std::va_list args;
    va_start(args, format);
    fatalHandler(file, line, format, args);
    va_end(args);
}

/**
 * 注册致命错误回调函数（不具备线程安全性，建议只在初始化的时候调用）。
 * 如果不调用这个函数，那么将不会有额外的处理，直接调用 terminate。
 * @param callback 回调函数
 */
export void registerFatalHandler(FatalHandlerCallBack callback) noexcept;

/**
 * 断言失败处理函数指针类型
 */
export using AssertFailedHandler =
    void(*)(const char* file, int line, const char* func_name, const char* condition_name, const char* message);

/**
 * 断言失败处理函数指针（编译期初始化为默认的断言失败处理函数）
 */
constinit AssertFailedHandler assertFailedHandler = nullptr;

/**
 * 断言入口
 * @attention 不应直接调用这个函数，应当使用宏 @ref EMDEVIF_ASSERT，可以自动填充文件名、行号、函数名与表达式名称，
 * 但需要引入头文件 @ref emdevif/fatal_handler.hpp
 * @note 为了避免与 <cassert> 中的 assert 冲突，故将此函数命名为 emdevif_assert
 *
 * @param condition 待断言的表达式
 * @param file 调用函数所在文件
 * @param line 调用函数所在行号
 * @param func_name 包含函数名称
 * @param condition_name 表达式名称
 * @param message 信息
 */
export inline void emdevif_assert(const bool condition,
                                  const char* file,
                                  const int line,
                                  const char* func_name,
                                  const char* condition_name,
                                  const char* message = "") noexcept
{
    if (!condition) {
        if (assertFailedHandler != nullptr) {
            assertFailedHandler(file, line, func_name, condition_name, message);
        }

        terminate();
    }
}

/**
 * 断言失败处理函数注册（不具备线程安全性，建议只在初始化的时候调用）。
 * 如果不调用这个函数，那么将调用默认的断言失败处理函数。
 * @param handler 回调函数
 */
export void registerAssertFailedHandler(AssertFailedHandler handler) noexcept;

}  // namespace emdevif
