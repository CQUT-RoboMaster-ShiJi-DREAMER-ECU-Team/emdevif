/**
 * @file ErrorMessage.cppm
 * @author DuYicheng
 * @date 2025-09-06
 * @brief 错误信息
 */

module;

#include <cstddef>
#include <cstring>

#include <string_view>

#ifndef EMDEVIF_ERROR_HANDLER_ERROR_MESSAGE_BUFFER_MAX_LENGTH
#define EMDEVIF_ERROR_HANDLER_ERROR_MESSAGE_BUFFER_MAX_LENGTH 128
#endif

export module emdevif.error_handler:ErrorMessage;

namespace emdevif {

constexpr std::size_t error_message_buffer_length = EMDEVIF_ERROR_HANDLER_ERROR_MESSAGE_BUFFER_MAX_LENGTH;

export template<std::size_t max_length, typename Allocator = std::allocator<char>>
class ErrorMessage
{
public:
    constexpr ErrorMessage() noexcept  // NOLINT(*-pro-type-member-init)
    {
        clear();
    }

    constexpr ErrorMessage& operator=(const ErrorMessage&) = default;

    ErrorMessage& operator=(const std::string_view sv) noexcept
    {
        clear();

        const auto buffer_len = capacity();
        const auto sv_len = sv.length();
        const auto min_len = std::min(buffer_len, sv_len);

        std::strncpy(buffer_, sv.data(), min_len);
        buffer_[min_len] = '\0';

        return *this;
    }

    explicit ErrorMessage(const std::string_view sv) noexcept  // NOLINT(*-pro-type-member-init)
    {
        this->operator=(sv);
    }

    constexpr void clear() noexcept
    {
        buffer_[0] = '\0';
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return std::strlen(buffer_);
    }

    [[nodiscard]] std::size_t size_byte() const noexcept
    {
        return sizeof(char) * size();
    }

    [[nodiscard]] std::size_t length() const noexcept
    {
        return size();
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    [[nodiscard]] constexpr std::size_t capacity() const noexcept
    {
        return max_length;
    }

    char* data() noexcept
    {
        return buffer_;
    }

    [[nodiscard]] const char* c_str() const noexcept
    {
        return buffer_;
    }

    ErrorMessage& operator<<(const std::string_view sv) noexcept
    {
        const auto buffer_remain = capacity() - length();
        if (buffer_remain <= 1) {
            return *this;
        }

        const auto sv_len = sv.length();
        const auto min_len = std::min(buffer_remain, sv_len);

        std::strncat(buffer_, sv.data(), min_len);

        return *this;
    }

private:
    char buffer_[max_length];
};

export template<typename Allocator>
class ErrorMessage<static_cast<std::size_t>(-1), Allocator>
{
};

export ErrorMessage<error_message_buffer_length> err_msg;

}  // namespace emdevif
