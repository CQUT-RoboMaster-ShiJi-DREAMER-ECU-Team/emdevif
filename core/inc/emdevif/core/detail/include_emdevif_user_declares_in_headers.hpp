/**
 * @file include_emdevif_user_declares_in_headers.hpp
 * @brief
 */

// 开发者提示: 该头文件通常应该在
//     #ifndef EMDEVIF_MODULE_INTERFACE_UNIT ... #endif
// 块中被包含。如果发现不是这样的，极有可能设计有误。

#pragma once
#ifndef EMDEVIF_CORE_INCLUDE_EMDEVIF_USER_DECLARES_IN_HEADERS_HPP
#define EMDEVIF_CORE_INCLUDE_EMDEVIF_USER_DECLARES_IN_HEADERS_HPP

#ifndef EMDEVIF_USER_DECLARES_PROVIDE_MODULE
#include "emdevif/user_declares.hpp"
#else
#error \
    "If you use lib `emdevif` without module, you can not provide \
`emdevif_user_declares` as module(If use header files, \
lib `emdevif_user_declares` should be headers too)."
#endif

#endif  // !EMDEVIF_CORE_INCLUDE_EMDEVIF_USER_DECLARES_IN_HEADERS_HPP
