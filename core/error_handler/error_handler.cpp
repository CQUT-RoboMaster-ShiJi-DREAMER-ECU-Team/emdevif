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

EMDEVIF_NO_RETURN void defaultFatalHandler(const char* file, const int line, const char* message)
{
    EMDEVIF_UNUSED(file);
    EMDEVIF_UNUSED(line);
    EMDEVIF_UNUSED(message);

    while (true) {
    }
}

void registerFatalHandler(const FatalHandlerCallBack callback)
{
    if (callback != nullptr) {
        fatalHandlerCallback = callback;
    }
}

}  // namespace emdevif
