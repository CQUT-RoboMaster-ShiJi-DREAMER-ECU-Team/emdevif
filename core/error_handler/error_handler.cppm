/**
 * @file error_handler.cppm
 * @module rmdev.error_handler
 * @author 杜以成
 * @date 2025-06-01
 * @brief 错误处理
 */

module;

#include <cstdint>

#include "rmdev/attributes_and_useful_macros.h"

export module rmdev.error_handler;

namespace rmdev {

/**
 * 错误码
 */
export enum class ErrorCode : std::int8_t {
    Success = 0,            ///< 成功
    UnknownError = -1,      ///< 未知错误
    InvalidArgument = -2,   ///< 无效参数
    OutOfMemory = -3,       ///< 内存不足
    NotFound = -4,          ///< 未找到
    PermissionDenied = -5,  ///< 权限被拒绝
    Timeout = -6,           ///< 超时
    Full = -7,              ///< 已满
    Empty = -8,             ///< 为空
    AlreadyExists = -9,     ///< 已存在
    NotImplemented = -10,   ///< 未实现
    InternalError = -11     ///< 内部错误
};

/**
 * 致命错误回调函数指针
 */
using FaultHandlerCallBack = void (*)(const char* file, int line, const char* message);

/**
 * 默认的致命错误处理函数
 * @param file 所在文件名
 * @param line 所在行号
 * @param message 错误消息
 */
RMDEV_NO_RETURN void defaultFaultHandler(const char* file, int line, const char* message);

/**
 * 致命错误回调函数（编译期初始化为默认的回调函数）
 */
constinit FaultHandlerCallBack faultHandlerCallback = defaultFaultHandler;

/**
 * 致命错误处理函数
 * @note 建议使用宏 RMDEV_FAULT_HANDLER，可以自动填充文件名与行号，但需要
 * 引入头文件 rmdev/fault_handler.hpp
 * @param file 所在文件名
 * @param line 所在行号
 * @param message 错误信息
 */
export void faultHandler(const char* file, const int line, const char* message = "")
{
    faultHandlerCallback(file, line, message);
}

/**
 * 注册致命错误回调函数（不具备线程安全性，建议只在初始化的时候调用）。
 * 如果不调用这个函数，那么将采用默认的致命错误回调函数
 * @param callback 回调函数
 */
export void registerFaultHandler(FaultHandlerCallBack callback);

}  // namespace rmdev
