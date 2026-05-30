/**
 * @file logger.cppm
 * @brief C++20 模块接口文件，重导出 emdevif.logger 模块的所有声明。
 */

module;

#include "emdevif/core/attributes_and_useful_macros.h"
#include "emdevif/core/fatal_handler.h"
#include "emdevif/core/line_separator.h"

#include <cstdint>
#include <cstdarg>

#include <array>
#include <utility>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.logger;

import emdevif.core.error_handler;
import emdevif.user_declares;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/logger.hpp"