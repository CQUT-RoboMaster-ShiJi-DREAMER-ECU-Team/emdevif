/**
 * @file error_handler.cppm
 * @brief 错误处理
 */

module;

#include "emdevif/core/attributes_and_useful_macros.h"

#include <cstdint>
#include <cstdarg>

#include <concepts>
#include <compare>
#include <source_location>
#include <exception>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.error_handler;

import emdevif.core.concepts;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/error_handler.hpp"
