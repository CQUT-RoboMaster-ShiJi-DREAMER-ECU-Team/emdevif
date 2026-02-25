/**
 * @file sync.hpp
 * @brief
 */

// ReSharper disable CppNonInlineFunctionDefinitionInHeaderFile

#pragma once
#ifndef EMDEVIF_LOGGER_SRC_SYNC_HPP_
#define EMDEVIF_LOGGER_SRC_SYNC_HPP_

namespace emdevif::logger::detail::sync {

ErrorCode logInit(const VsnprintfImpl vsprintf_impl) noexcept
{
    detail::vsnprintf_impl_ = vsprintf_impl;
    return ErrorCode::Success;
}

void logDeInit() noexcept
{
    detail::vsnprintf_impl_ = nullptr;
}

void logImpl(const LoggerLevel level, const char* format, std::va_list args) noexcept
{
    auto* const p_buffer = reinterpret_cast<char*>(detail::log_msg_buffer_);

    const auto index1 = detail::snprintf_(p_buffer,
                                          logMsgBufferLength(),
                                          "%-10zu %7s ",
                                          ::emdevif::user_declares::logger::getTimeLine(),
                                          toCString(level));
    if (index1 < 0) {
        return;
    }

    const auto index2 = detail::vsnprintf_impl_(p_buffer + index1, logMsgBufferLength() - index1, format, args);
    if (index2 < 0) {
        return;
    }

    const std::size_t msg_len = index1 + index2;

#if (EMDEVIF_LINE_SEPARATOR_MODE == EMDEVIF_LINE_SEPARATOR_MODE_LF)
    if (msg_len + 1 < logMsgBufferLength()) {
        p_buffer[msg_len] = '\n';
        p_buffer[msg_len + 1] = '\0';
    }
    else {
        p_buffer[logMsgBufferLength() - 1] = '\0';
    }
#elif (EMDEVIF_LINE_SEPARATOR_MODE == EMDEVIF_LINE_SEPARATOR_MODE_CR)
    if (msg_len + 1 < logMsgBufferLength()) {
        p_buffer[msg_len] = '\r';
        p_buffer[msg_len + 1] = '\0';
    }
    else {
        p_buffer[logMsgBufferLength() - 1] = '\0';
    }
#else
    if (msg_len + 2 < logMsgBufferLength()) {
        p_buffer[msg_len] = '\r';
        p_buffer[msg_len + 1] = '\n';
        p_buffer[msg_len + 2] = '\0';
    }
    else {
        p_buffer[logMsgBufferLength() - 1] = '\0';
    }
#endif
}

}  // namespace emdevif::logger::detail::sync

#endif  // !EMDEVIF_LOGGER_SRC_SYNC_HPP_
