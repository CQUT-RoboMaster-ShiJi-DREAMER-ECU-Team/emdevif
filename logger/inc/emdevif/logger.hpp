/**
 * @file logger.hpp
 * @brief 日志
 */

#pragma once
#ifndef EMDEVIF_LOGGER_HPP
#define EMDEVIF_LOGGER_HPP

#include "emdevif/logger/config.hpp"
#include "emdevif/logger/config_values.hpp"

#if (EMDEVIF_LOGGER_MODE == 0)
namespace emdevif::logger::detail {
using VsnprintfImpl = int (*)(char* dest, std::size_t buffer_size, const char* format, std::va_list args);
}

#include "emdevif/logger/sync.hpp"
#elif (EMDEVIF_LOGGER_MODE == 1)
namespace emdevif::logger::detail {
using VsnprintfImpl = int (*)(char* dest, std::size_t buffer_size, const char* format, std::va_list args);
}

#include "emdevif/logger/async.hpp"
#elif (EMDEVIF_LOGGER_MODE == 2)
#include "emdevif/logger/external_impl.hpp"
#endif

#endif  // !EMDEVIF_LOGGER_HPP
