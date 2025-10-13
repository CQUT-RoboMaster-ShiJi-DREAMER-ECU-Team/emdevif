/**
 * @file Spi.cppm
 * @module emdevif.peripheral.spi
 * @author DuYicheng
 * @date 2025-10-04
 * @brief SPI 通信
 */

module;

#include <cstdint>

#include <limits>
#include <span>
#include <string_view>
#include <type_traits>

#include "emdevif/fatal_handler.hpp"

export module emdevif.peripheral.spi;

export import emdevif.errorHandler;
import emdevif.peripheralHandleMap;
import emdevif.peripheral.base;
import emdevif.peripheral.model.spi;

export namespace emdevif {

class Spi : public SpiModel
{
private:
    SpiModel::Instance* instance_;

public:
    Spi() = delete;

    explicit constexpr Spi(const std::string_view name) noexcept
        : instance_(static_cast<SpiModel::Instance*>(PeripheralHandleMap::findHandle(name).value_or(nullptr)))
    {
        internal::PeripheralErrorHandler::checkInstanceIsExist(instance_, "SPI");
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

        return instance_->transmit_receive_function_(in_isr, instance_->handle_, tx_data, rx_data, timeout_ms);
    }

    static constexpr uint32_t max_delay = std::numeric_limits<uint32_t>::max();
};

}  // namespace emdevif
