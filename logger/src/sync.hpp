/**
 * @file sync.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_LOGGER_SRC_SYNC_HPP_
#define EMDEVIF_LOGGER_SRC_SYNC_HPP_

namespace emdevif::logger::detail::sync {

static VsprintfImpl vsprintf_impl_;

ErrorCode logInit(VsprintfImpl vsprintf_impl) noexcept
{
    vsprintf_impl_ = vsprintf_impl;
    return ErrorCode::Success;
}

}  // namespace emdevif::logger::detail::sync

#endif  // !EMDEVIF_LOGGER_SRC_SYNC_HPP_
