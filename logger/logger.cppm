/**
 * @file logger.cppm
 * @brief C++20 模块接口文件，重导出 emdevif.logger 模块的所有声明。
 */

module;

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.logger;

#include "emdevif/core/attributes_and_useful_macros.h"
#include "emdevif/core/fatal_handler.h"
#include "emdevif/core/line_separator.h"
#include "emdevif/logger.hpp"
