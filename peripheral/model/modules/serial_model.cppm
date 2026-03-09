/**
 * @file serial_model.cppm
 * @brief 串口模型
 */

module;

#include <cstdint>

#include <type_traits>
#include <span>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.peripheral.model.serial;

import emdevif.core.error_handler;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/peripheral/model/serial.hpp"
