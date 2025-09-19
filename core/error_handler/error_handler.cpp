/**
 * @file error_handler.cpp
 * @author 杜以成
 * @date 2025-06-01
 * @brief 错误处理
 */

module;

#include "emdevif/attributes_and_useful_macros.h"

module emdevif.errorHandler;

namespace emdevif {

void registerFatalHandler(const FatalHandlerCallBack callback)
{
    if (callback != nullptr) {
        fatalHandlerCallback = callback;
    }
}

EMDEVIF_NO_RETURN void defaultTerminateFunction()
{
    while (true) {
    }
}

void registerTerminateFunction(const TerminateFunction func)
{
    if (func != nullptr) {
        terminateFunction = func;
    }
}

void defaultAssertFailedHandler(const char* file,
                                const int line,
                                const char* func_name,
                                const char* condition_name,
                                const char* message)
{
    EMDEVIF_UNUSED(file);
    EMDEVIF_UNUSED(line);
    EMDEVIF_UNUSED(func_name);
    EMDEVIF_UNUSED(condition_name);
    EMDEVIF_UNUSED(message);

    terminate();
}

void registerAssertFailedHandler(const AssertFailedHandler handler)
{
    if (handler != nullptr) {
        assertFailedHandler = handler;
    }
}

}  // namespace emdevif
