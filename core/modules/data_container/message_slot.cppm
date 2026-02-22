/**
 * @file message_slot.cppm
 * @brief 消息槽
 */

module;

#include <cstddef>

#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.data_container.message_slot;

import emdevif.core.data_container.message_queue;
import emdevif.core.error_handler;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/data_container/message_slot.hpp"
