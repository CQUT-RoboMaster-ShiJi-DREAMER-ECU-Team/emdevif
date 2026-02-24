/**
 * @file timeline.cppm
 * @brief 时间戳
 */

module;

#ifndef EMDEVIF_USER_DECLARES_PROVIDE_MODULE
#include "emdevif/user_declares.hpp"
#endif

#include <cstdint>

#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.timeline;

#ifdef EMDEVIF_USER_DECLARES_PROVIDE_MODULE
import emdevif.userDeclares;
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/timeline.hpp"
