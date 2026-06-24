/**
 * @file sync.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_LOGGER_ASYNC_HPP
    #define EMDEVIF_LOGGER_ASYNC_HPP

    #include "emdevif/logger/config.hpp"

        #include "emdevif/core/fatal_handler.h"
        #include "emdevif/core/error_handler.hpp"

        #include "emdevif/core/detail/include_emdevif_user_declares_in_headers.hpp"

        #include <cstdarg>

namespace emdevif::logger::detail {

namespace async {

ErrorCode logInit(VsnprintfImpl vsprintf_impl) noexcept;
void logDeInit() noexcept;
void logImpl(LoggerLevel level, const char* format, std::va_list args) noexcept;

}  // namespace async

inline ErrorCode logInit(const VsnprintfImpl vsprintf_impl) noexcept
{
    return async::logInit(vsprintf_impl);
}

inline void logDeInit() noexcept
{
    async::logDeInit();
}

inline void logImpl(const LoggerLevel level, const char* format, std::va_list args) noexcept
{
    async::logImpl(level, format, args);
}

}  // namespace emdevif::logger::detail

    #include "emdevif/logger/sync_async_interface.hpp"

#endif  // !EMDEVIF_LOGGER_ASYNC_HPP
