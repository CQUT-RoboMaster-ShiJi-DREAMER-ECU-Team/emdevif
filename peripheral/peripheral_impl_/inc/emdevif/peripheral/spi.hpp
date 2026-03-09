/**
 * @file spi.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_SPI_HPP
    #define EMDEVIF_PERIPHERAL_SPI_HPP

    #include "emdevif/core/detail/config.hpp"
    #include "emdevif/peripheral/detail/peripheral_error_handler.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/peripheral/peripheral_handle_map.hpp"
        #include "emdevif/peripheral/model/spi.hpp"
        #include "emdevif/core/error_handler.hpp"

        #include <cstdint>

        #include <limits>
        #include <span>
        #include <string_view>
        #include <type_traits>

        #include "emdevif/core/fatal_handler.h"
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

class Spi : public SpiModel
{
private:
    SpiModel::Instance* instance_;

public:
    Spi() = delete;

    explicit constexpr Spi(const std::string_view name) noexcept
        : instance_(static_cast<SpiModel::Instance*>(PeripheralHandleMap::findHandle(name)))
    {
        detail::PeripheralErrorHandler::checkInstanceIsExist(instance_, "SPI", name);
    }

    // NOLINTNEXTLINE
    ErrorCode transmitReceive(const bool in_isr,
                              const std::span<const uint8_t> tx_data,
                              const std::span<uint8_t> rx_data,
                              const uint32_t timeout_ms) const noexcept
    {
        if (tx_data.size() != rx_data.size()) {
            return ErrorCode::InvalidArgument;
        }

        return instance_->transmit_receive_function(in_isr, instance_->handle, tx_data, rx_data, timeout_ms);
    }

    static constexpr uint32_t max_delay = std::numeric_limits<uint32_t>::max();
};

}  // namespace emdevif

#endif  // !EMDEVIF_PERIPHERAL_SPI_HPP
