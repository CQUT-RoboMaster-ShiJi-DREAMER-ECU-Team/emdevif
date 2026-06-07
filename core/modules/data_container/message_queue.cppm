/**
 * @file message_queue.cppm
 * @brief 消息队列接口
 */

module;

#include <cstdint>
#include <cstddef>

#include <concepts>
#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.core.data_container.message_queue;

import emdevif.core.error_handler;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/core/data_container/message_queue.hpp"
