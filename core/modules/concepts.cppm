/**
 * @file concepts.cppm
 * @brief 概念
 */

module;

#include <type_traits>
#include <concepts>
#include <compare>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.concepts;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/concepts.hpp"
