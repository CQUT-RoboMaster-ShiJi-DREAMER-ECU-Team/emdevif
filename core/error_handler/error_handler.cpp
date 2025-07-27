/**
 * @file error_handler.cpp
 * @author 杜以成
 * @date 2025-06-01
 * @brief 错误处理
 */

module;

#include "rmdev/attributes_and_useful_macros.h"

module rmdev.error_handler;

namespace rmdev {

RMDEV_NO_RETURN void defaultFaultHandler(const char* file, const int line, const char* message)
{
    RMDEV_UNUSED(file);
    RMDEV_UNUSED(line);
    RMDEV_UNUSED(message);

    while (true) {
    }
}

void registerFaultHandler(const FaultHandlerCallBack callback)
{
    if (callback != nullptr) {
        faultHandlerCallback = callback;
    }
}

}  // namespace rmdev
