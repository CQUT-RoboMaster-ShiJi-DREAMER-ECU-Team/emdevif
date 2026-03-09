/**
 * @file can_model.cppm
 * @brief CAN 通信模型
 */

module;

#include <cstdint>

#include <span>
#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.peripheral.model.can;

import emdevif.core.error_handler;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/peripheral/model/can.hpp"
