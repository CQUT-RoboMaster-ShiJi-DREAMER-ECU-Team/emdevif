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

export namespace emdevif {

class Spi
{
public:
    Spi() = delete;
    EMDEVIF_DELETE_COPY_CONSTRUCTOR(Spi);

    using ReceiveTransmitFunction = ErrorCode (&)(bool in_isr,
                                                  void* handle,
                                                  std::span<const uint8_t> tx_data,
                                                  std::span<uint8_t> rx_data,
                                                  uint32_t timeout_ms);

private:
    void* handle_;

    const ReceiveTransmitFunction receive_transmit_function_;

private:
    // 用于在编译期求值时提供报错信息
    static void ThisIsACompileTimeMessage_CouldNotFoundHandle() {}

public:
    constexpr Spi(const std::string_view name, ReceiveTransmitFunction receive_transmit_function) noexcept
        : handle_(PeripheralHandleMap::findHandle(name).value_or(nullptr)),
          receive_transmit_function_(receive_transmit_function)
    {
        if (handle_ == nullptr) {
            if (std::is_constant_evaluated()) {
                ThisIsACompileTimeMessage_CouldNotFoundHandle();
                return;
            }

            using namespace std::literals;

            constexpr auto begin_str = "Could not find the SPI handle named \""sv;
            constexpr auto end_str = "\"."sv;

            err_msg.clear();
            err_msg << begin_str << name << end_str;

            EMDEVIF_FATAL_HANDLER(err_msg.c_str());
            err_msg.clear();
        }
    }

    ErrorCode receiveTransmit(const bool in_isr,
                              const std::span<const uint8_t> tx_data,
                              const std::span<uint8_t> rx_data,
                              const uint32_t timeout_ms) const noexcept
    {
        if (tx_data.size() != rx_data.size()) {
            return ErrorCode::InvalidArgument;
        }

        return receive_transmit_function_(in_isr, handle_, tx_data, rx_data, timeout_ms);
    }
};

}  // namespace emdevif
