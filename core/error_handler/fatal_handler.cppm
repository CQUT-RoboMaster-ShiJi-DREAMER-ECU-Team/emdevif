/**
 * @file fatal_handler.cppm
 * @module emdevif.error_handler:fatal_handler
 * @author DuYicheng
 * @date 2025-09-05
 * @brief 致命错误处理函数
 */

module;

#include "emdevif/attributes_and_useful_macros.h"

#ifndef EMDEVIF_ERROR_HANDLER_FATAL_HANDLER_MESSAGE_BUFFER_LENGTH
#define EMDEVIF_ERROR_HANDLER_FATAL_HANDLER_MESSAGE_BUFFER_LENGTH 0
#endif

export module emdevif.errorHandler:fatalHandler;

namespace emdevif {

#if (EMDEVIF_ERROR_HANDLER_FATAL_HANDLER_MESSAGE_BUFFER_LENGTH == 0)

#define EMDEVIF_FATAL_DYNAMIC_ALLOCATE true

#else

#define EMDEVIF_FATAL_DYNAMIC_ALLOCATE false

char emdevif_fatal_message_buffer[EMDEVIF_ERROR_HANDLER_FATAL_HANDLER_MESSAGE_BUFFER_LENGTH] = {'\0'};

#endif

/**
 * 致命错误回调函数指针
 */
using FatalHandlerCallBack = void (*)(const char* file, int line, const char* message);

/**
 * 默认的致命错误处理函数
 * @param file 所在文件名
 * @param line 所在行号
 * @param message 错误消息
 */
EMDEVIF_NO_RETURN void defaultFatalHandler(const char* file, int line, const char* message);

/**
 * 致命错误回调函数（编译期初始化为默认的回调函数）
 */
constinit FatalHandlerCallBack fatalHandlerCallback = defaultFatalHandler;

/**
 * 致命错误处理函数
 * @note 建议使用宏 EMDEVIF_FATAL_HANDLER，可以自动填充文件名与行号，但需要
 * 引入头文件 emdevif/fatal_handler.hpp
 * @param file 所在文件名
 * @param line 所在行号
 * @param message 错误信息
 */
export void fatalHandler(const char* file, const int line, const char* message = "")
{
    fatalHandlerCallback(file, line, message);
}

/**
 * 注册致命错误回调函数（不具备线程安全性，建议只在初始化的时候调用）。
 * 如果不调用这个函数，那么将采用默认的致命错误回调函数
 * @param callback 回调函数
 */
export void registerFatalHandler(FatalHandlerCallBack callback);

}  // namespace emdevif
