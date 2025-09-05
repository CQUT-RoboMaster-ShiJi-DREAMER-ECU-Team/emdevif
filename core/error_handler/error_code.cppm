/**
 * @file error_code.cppm
 * @module emdevif.error_handler:error_code
 * @author DuYicheng
 * @date 2025-09-05
 * @brief 错误码
 */

module;

#include <cstdint>

export module emdevif.error_handler:error_code;

namespace emdevif {

/**
 * 错误码
 */
export enum class ErrorCode : std::int_fast8_t {
    Success = 0,            ///< 成功
    UnknownError = -1,      ///< 未知错误
    InvalidArgument = -2,   ///< 无效参数
    OutOfMemory = -3,       ///< 内存不足
    NotFound = -4,          ///< 未找到
    PermissionDenied = -5,  ///< 权限被拒绝
    Timeout = -6,           ///< 超时
    Full = -7,              ///< 已满
    Empty = -8,             ///< 为空
    OperationFail = -9,     ///< 操作失败
    AlreadyExists = -10,    ///< 已存在
    NotImplemented = -11,   ///< 未实现
    InternalError = -12     ///< 内部错误
};

}  // namespace emdevif
