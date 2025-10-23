/**
 * @file error_handler.cpp
 * @author 杜以成
 * @date 2025-06-01
 * @brief 错误处理
 */

module;

#include <cstdarg>

#include "emdevif/attributes_and_useful_macros.h"

module emdevif.errorHandler;

namespace emdevif {

EMDEVIF_NO_RETURN void defaultTerminateFunction() noexcept
{
    while (true) {
    }
}

void registerTerminateFunction(const TerminateFunction func) noexcept
{
    if (func != nullptr) {
        terminateFunction = func;
    }
}

EMDEVIF_NO_RETURN void defaultFatalHandler(const char* file,
                                           const int line,
                                           const char* format,
                                           const std::va_list args) noexcept
{
    EMDEVIF_UNUSED(file);
    EMDEVIF_UNUSED(line);
    EMDEVIF_UNUSED(format);
    EMDEVIF_UNUSED(args);

    terminate();
}

void registerFatalHandler(const FatalHandlerCallBack callback) noexcept
{
    if (callback != nullptr) {
        fatalHandlerCallback = callback;
    }
}

void defaultAssertFailedHandler(const char* file,
                                const int line,
                                const char* func_name,
                                const char* condition_name,
                                const char* message) noexcept
{
    EMDEVIF_UNUSED(file);
    EMDEVIF_UNUSED(line);
    EMDEVIF_UNUSED(func_name);
    EMDEVIF_UNUSED(condition_name);
    EMDEVIF_UNUSED(message);

    terminate();
}

void registerAssertFailedHandler(const AssertFailedHandler handler) noexcept
{
    if (handler != nullptr) {
        assertFailedHandler = handler;
    }
}

}  // namespace emdevif
