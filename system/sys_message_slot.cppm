/**
 * @file sys_message_slot.cppm
 * @brief 系统消息槽 - 由操作系统提供的队列实现的消息槽
 */

module;

#include <utility>
#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.sys.sysMessageSlot;

import emdevif.sys.thread;
import emdevif.sys.sysQueue;
import emdevif.core.data_container.message_slot;
import emdevif.core.data_container.message_queue;
import emdevif.core.error_handler;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/system/sys_message_slot.hpp"
