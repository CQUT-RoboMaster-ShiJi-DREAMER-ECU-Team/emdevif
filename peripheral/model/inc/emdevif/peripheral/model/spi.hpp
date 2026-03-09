/**
 * @file spi.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_MODEL_SPI_HPP
    #define EMDEVIF_PERIPHERAL_MODEL_SPI_HPP

#include "emdevif/core/detail/config.hpp"

#ifndef EMDEVIF_MODULE_INTERFACE_UNIT
    #include "emdevif/core/error_handler.hpp"

    #include <cstdint>

    #include <span>
    #include <type_traits>
#endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

class SpiModel
{
public:
    using TransmitReceiveFunction = ErrorCode (*)(bool in_isr,
                                                  void* handle,
                                                  std::span<const uint8_t> tx_data,
                                                  std::span<uint8_t> rx_data,
                                                  uint32_t timeout_ms);

    struct Instance {
        void* const handle{nullptr};

        const TransmitReceiveFunction transmit_receive_function{nullptr};
    };
};

static_assert(std::is_standard_layout_v<SpiModel::Instance>);

}  // namespace emdevif


#endif  // !EMDEVIF_PERIPHERAL_MODEL_SPI_HPP
