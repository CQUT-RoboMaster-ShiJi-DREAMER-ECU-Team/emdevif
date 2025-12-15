/**
 * @file error_code.cppm
 * @brief 错误码
 */

module;

#include <cstdint>

#include <concepts>
#include <compare>
#include <source_location>

export module emdevif.errorHandler:errorCode;
import :fatalHandlerAndAssert;

import emdevif.concepts;

namespace emdevif {

/**
 * 错误码
 */
export class ErrorCode
{
public:
    /// 错误码的值
    enum ErrorCodeValue : std::int_fast8_t {
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

private:
    ErrorCodeValue value_{};  ///< 内部存储的错误值

public:
    constexpr ErrorCode() noexcept = default;
    ~ErrorCode() noexcept = default;

    // ReSharper disable once CppNonExplicitConvertingConstructor
    constexpr ErrorCode(const ErrorCodeValue value) noexcept : value_(value) {}  // NOLINT(*-explicit-constructor)

    template<std::integral IntType>
    explicit constexpr ErrorCode(const IntType& value) noexcept : value_(value)
    {
    }

    template<std::integral IntType>
    explicit constexpr operator IntType() const noexcept
    {
        return value_;
    }

    constexpr std::strong_ordering operator<=>(const ErrorCode&) const noexcept = default;

    /**
     * 如果未成功（值不是 ErrorCode::Success），终止程序
     * @param sl source_location，默认值为 std::source_location::current()
     */
    void terminateIfNotSucceed(const std::source_location sl = std::source_location::current()) const noexcept
    {
        if (value_ != Success) {
            fatalHandler(sl.file_name(), sl.line(), "Error happened with error code is %d.", static_cast<int>(value_));
        }
    }
};

/**
 * 如果未成功（值不是 ErrorCode::Success），终止程序
 * @param error_code 错误码
 * @param sl source_location，默认值为 std::source_location::current()
 */
export void terminateIfNotSucceed(const ErrorCode& error_code,
                                  const std::source_location sl = std::source_location::current()) noexcept
{
    error_code.terminateIfNotSucceed(sl);
}

/**
 * 如果是空指针，终止程序
 * @tparam Pointer 指针类型
 * @param ptr 指针的值
 * @param sl source_location，默认值为 std::source_location::current()
 */
export template<emdevif::PointerType Pointer>
void terminateIfNullptr(Pointer ptr, const std::source_location sl = std::source_location::current()) noexcept
{
    if (ptr == nullptr) {
        fatalHandler(sl.file_name(), sl.line(), "Null pointer provided.");
    }
}

}  // namespace emdevif
