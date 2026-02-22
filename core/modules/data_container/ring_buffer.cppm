/**
 * @file ring_buffer.cppm
 * @brief 环形缓冲区
 */

module;

#include <cstdint>
#include <cstring>

#include <array>
#include <concepts>
#include <span>
#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.data_container.ring_buffer;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/data_container/ring_buffer.hpp"
