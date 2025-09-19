/**
 * @file fatal_handler.cppm
 * @module emdevif.error_handler:fatalHandlerAndAssert
 * @author DuYicheng
 * @date 2025-09-05
 * @brief 致命错误处理函数与断言
 */

module;

#include "emdevif/attributes_and_useful_macros.h"

export module emdevif.errorHandler:fatalHandlerAndAssert;

namespace emdevif {

/**
 * 默认的程序终止函数
 */
EMDEVIF_NO_RETURN void defaultTerminateFunction();

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
export void registerTerminateFunction(TerminateFunction func);

/**
 * 致命错误回调函数指针
 */
export using FatalHandlerCallBack = void(*)(const char* file, int line, const char* message);

/**
 * 致命错误回调函数（编译期初始化为默认的回调函数）
 */
constinit FatalHandlerCallBack fatalHandlerCallback = nullptr;

/**
 * 致命错误处理函数
 * @note 建议使用宏 EMDEVIF_FATAL_HANDLER，可以自动填充文件名与行号，但需要
 * 引入头文件 emdevif/fatal_handler.hpp
 * @param file 所在文件名
 * @param line 所在行号
 * @param message 错误信息
 */
export EMDEVIF_NO_RETURN void fatalHandler(const char* file, const int line, const char* message = "")
{
    if (fatalHandlerCallback != nullptr) {
        fatalHandlerCallback(file, line, message);
    }

    terminate();
}

/**
 * 注册致命错误回调函数（不具备线程安全性，建议只在初始化的时候调用）。
 * 如果不调用这个函数，那么将不会有额外的处理，直接调用 terminate。
 * @param callback 回调函数
 */
export void registerFatalHandler(FatalHandlerCallBack callback);

/**
 * 默认的断言失败处理函数
 * @param file 调用函数所在文件
 * @param line 调用函数所在行号
 * @param func_name 包含函数名称
 * @param condition_name 表达式名称
 * @param message 信息
 */
void defaultAssertFailedHandler(const char* file,
                                int line,
                                const char* func_name,
                                const char* condition_name,
                                const char* message);

/**
 * 断言失败处理函数指针类型
 */
export using AssertFailedHandler =
    void(*)(const char* file, int line, const char* func_name, const char* condition_name, const char* message);

/**
 * 断言失败处理函数指针（编译期初始化为默认的断言失败处理函数）
 */
constinit AssertFailedHandler assertFailedHandler = defaultAssertFailedHandler;

/**
 * 断言入口
 * @note @arg 不应直接调用这个函数，应当使用宏 EMDEVIF_ASSERT，可以自动填充文件名、行号、函数名与表达式名称，
 * 但需要引入头文件 emdevif/fatal_handler.hpp
 *       @arg 为了避免与 <cassert> 中的 assert 冲突，故将此函数命名为 emdevif_assert
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
                                  const char* message = "")
{
    if (!condition) {
        assertFailedHandler(file, line, func_name, condition_name, message);
    }
}

/**
 * 断言失败处理函数注册（不具备线程安全性，建议只在初始化的时候调用）。
 * 如果不调用这个函数，那么将调用默认的断言失败处理函数。
 * @param handler 回调函数
 */
export void registerAssertFailedHandler(AssertFailedHandler handler);

}  // namespace emdevif
