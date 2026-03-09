/**
 * @file spi_model.cppm
 * @brief SPI 通信模型
 */

module;

#include <cstdint>

#include <span>
#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module emdevif.peripheral.model.spi;

import emdevif.core.error_handler;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "emdevif/peripheral/model/spi.hpp"
