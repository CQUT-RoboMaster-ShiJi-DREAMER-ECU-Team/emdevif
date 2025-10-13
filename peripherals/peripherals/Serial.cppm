/**
 * @file Serial.cppm
 * @module emdevif.peripheralModels.Serial
 * @author 杜以成
 * @date 2025-05-09
 * @brief 串口通信
 */

module;

#include <cstdint>

#include <string_view>
#include <span>
#include <type_traits>

#include "emdevif/simplify_decl_macros.hpp"
#include "emdevif/fatal_handler.hpp"

export module emdevif.peripheralModels.serial;

export import emdevif.errorHandler;
import emdevif.peripheralHandleMap;
import emdevif.peripheralModels.errorHandler;

export namespace emdevif {

class Serial
{
public:
    Serial() = delete;

    using ReceiveFunction = ErrorCode (&)(bool in_isr,
                                          void* handle,
                                          std::span<uint8_t> received_data,
                                          uint32_t timeout_ms);

    using ReceiveCallback = ErrorCode (*)(bool in_isr, void* handle, std::span<uint8_t> received_data);

    using TransmitFunction = ErrorCode (&)(bool in_isr,
                                           void* handle,
                                           std::span<const uint8_t> data,
                                           uint32_t timeout_ms);

    struct BehaviourFunction {
        const ReceiveFunction receive_function{noReceive};
        ReceiveCallback receive_callback{nullptr};
        const TransmitFunction transmit_function{noTransmit};
    };

private:
    void* handle_;

    const ReceiveFunction receive_function_;
    const ReceiveCallback receive_callback_;

    const TransmitFunction transmit_function_;

public:
    constexpr Serial(const std::string_view name, const BehaviourFunction& behaviour_function) noexcept
        : handle_(PeripheralHandleMap::findHandle(name).value_or(nullptr)),
          receive_function_(behaviour_function.receive_function),
          receive_callback_(behaviour_function.receive_callback),
          transmit_function_(behaviour_function.transmit_function)
    {
        internal::checkHandleIsExist(handle_, "Serial");
    }

    ErrorCode receive(const bool in_isr,  // NOLINT(*-use-nodiscard)
                      std::span<uint8_t> received_data,
                      const uint32_t timeout_ms) const noexcept
    {
        return receive_function_(in_isr, handle_, received_data, timeout_ms);
    }

    ErrorCode receiveCallbackEntry(const bool in_isr, void* handle, std::span<uint8_t> received_data) const noexcept
    {
        if (receive_callback_ == nullptr) {
            return ErrorCode::NotImplemented;
        }

        return receive_callback_(in_isr, handle, received_data);
    }

    ErrorCode transmit(const bool in_isr,  // NOLINT(*-use-nodiscard)
                       const std::span<const uint8_t> transmit_data,
                       const uint32_t timeout_ms) const noexcept
    {
        const auto status = transmit_function_(in_isr, handle_, transmit_data, timeout_ms);
        return status;
    }

private:
    static ErrorCode noTransmit(bool, void*, std::span<const uint8_t>, uint32_t) noexcept;

    static ErrorCode noReceive(bool, void*, std::span<uint8_t>, uint32_t) noexcept;
};

}  // namespace emdevif

// 待编译器实现
// module :private;

namespace emdevif {

ErrorCode Serial::noTransmit(bool, void*, const std::span<const uint8_t>, uint32_t) noexcept
{
    return ErrorCode::NotImplemented;
}

ErrorCode Serial::noReceive(bool, void*, std::span<uint8_t>, uint32_t) noexcept
{
    return ErrorCode::NotImplemented;
}

}  // namespace emdevif
