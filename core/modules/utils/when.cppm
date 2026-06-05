/**
 * @file when.cppm
 * @brief
 */

module;

#include <utility>
#include <type_traits>
#include <concepts>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.utils.when;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/utils/when.hpp"
