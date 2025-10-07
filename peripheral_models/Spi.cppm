/**
 * @file Spi.cppm
 * @module emdevif.peripheralModels.spi
 * @author DuYicheng
 * @date 2025-10-04
 * @brief SPI 通信
 */

module;

#include <cstdint>

#include <span>
#include <string_view>
#include <type_traits>

#include "emdevif/simplify_decl_macros.hpp"
#include "emdevif/fatal_handler.hpp"

export module emdevif.peripheralModels.spi;

export import emdevif.errorHandler;
import emdevif.peripheralHandleMap;
import emdevif.peripheralModels.errorHandler;

export namespace emdevif {

class Spi
{
public:
    Spi() = delete;

    using TransmitReceiveFunction = ErrorCode (&)(bool in_isr,
                                                  void* handle,
                                                  std::span<const uint8_t> tx_data,
                                                  std::span<uint8_t> rx_data,
                                                  uint32_t timeout_ms);

private:
    void* handle_;

    const TransmitReceiveFunction transmit_receive_function_;

public:
    constexpr Spi(const std::string_view name, TransmitReceiveFunction receive_transmit_function) noexcept
        : handle_(PeripheralHandleMap::findHandle(name).value_or(nullptr)),
          transmit_receive_function_(receive_transmit_function)
    {
        internal::checkHandleIsExist(handle_, "SPI");
    }

    ErrorCode transmitReceive(const bool in_isr,
                              const std::span<const uint8_t> tx_data,
                              const std::span<uint8_t> rx_data,
                              const uint32_t timeout_ms) const noexcept
    {
        if (tx_data.size() != rx_data.size()) {
            return ErrorCode::InvalidArgument;
        }

        return transmit_receive_function_(in_isr, handle_, tx_data, rx_data, timeout_ms);
    }
};

}  // namespace emdevif
