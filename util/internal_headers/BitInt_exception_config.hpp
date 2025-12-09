/**
 * @file BitInt_exception_config.hpp
 * @brief emdevif.util.BitInt 模块的异常配置
 */

#pragma once
#ifndef EMDEVIF_UTIL_BITINT_EXCEPTION_CONFIG_HPP
#define EMDEVIF_UTIL_BITINT_EXCEPTION_CONFIG_HPP

#if (defined(EMDEVIF_ENABLE_EXCEPTIONS) && EMDEVIF_ENABLE_EXCEPTIONS)
#include <stdexcept>

#define EMDEVIF_UTIL_BITINT_NOEXCEPT
#else
#define EMDEVIF_UTIL_BITINT_NOEXCEPT noexcept
#endif

#endif  // !EMDEVIF_UTIL_BITINT_EXCEPTION_CONFIG_HPP
