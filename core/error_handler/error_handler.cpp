/**
 * @file error_handler.cpp
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

void registerFatalHandler(const FatalHandlerCallBack callback) noexcept
{
    if (callback != nullptr) {
        fatalHandlerCallback = callback;
    }
}

void illFormatedCodeToAssertFailed() noexcept {}

void registerAssertFailedHandler(const AssertFailedHandler handler) noexcept
{
    if (handler != nullptr) {
        assertFailedHandler = handler;
    }
}

}  // namespace emdevif
