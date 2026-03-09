/**
 * @file InitOnce.cppm
 * @brief 单次初始化类
 */

module;

#include <type_traits>
#include <utility>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.util.init_once;

import emdevif.core.error_handler;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/util/init_once.hpp"
