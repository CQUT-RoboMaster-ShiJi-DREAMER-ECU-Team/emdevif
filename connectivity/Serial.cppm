/**
 * @file Serial.cppm
 * @module emdevif.connectivity.Serial
 * @author 杜以成
 * @date 2025-05-09
 * @brief 串口通信
 */

module;

#include <cstdint>

#include <string_view>
#include <span>

#include "emdevif/simplify_decl_macros.hpp"
#include "emdevif/fatal_handler.hpp"

export module emdevif.connectivity.Serial;

export import emdevif.error_handler;
import emdevif.PeripheralHandleMap;

export namespace emdevif {

class Serial
{
public:
    Serial() = delete;
    EMDEVIF_DELETE_COPY_CONSTRUCTOR(Serial);

    using ReceiveEntry = ErrorCode (*)(bool in_isr,
                                       void* handle,
                                       std::span<uint8_t> received_data,
                                       uint32_t timeout_ms);

    using ReceiveCallback = ErrorCode (*)(bool in_isr, void* handle, std::span<uint8_t> received_data);

    using TransmitFunction = ErrorCode (*)(bool in_isr, void* handle, std::span<uint8_t> data);

    struct BehaviourFunction {
        ReceiveEntry receive_entry{nullptr};
        ReceiveCallback receive_callback{nullptr};
        TransmitFunction transmit_function{nullptr};
    };

private:
    void* handle_;

    ReceiveEntry receive_entry_;
    ReceiveCallback receive_callback_;

    TransmitFunction transmit_function_;

public:
    Serial(const std::string_view name, const BehaviourFunction& behaviour_function) noexcept
        : handle_(PeripheralHandleMap::findHandle(name).value_or(nullptr)),
          receive_entry_(behaviour_function.receive_entry),
          receive_callback_(behaviour_function.receive_callback),
          transmit_function_(behaviour_function.transmit_function)
    {
        if (handle_ == nullptr) {
            using namespace std::literals;

            constexpr auto begin_str = "Could not find the serial handle named \""sv;
            constexpr auto end_str = "\"."sv;

            err_msg.clear();
            err_msg << begin_str << name << end_str;

            EMDEVIF_FATAL_HANDLER(err_msg.c_str());
            err_msg.clear();
        }
    }

    ErrorCode receive(const bool in_isr, std::span<uint8_t> received_data, const uint32_t timeout_ms)
    {
        auto status = receive_entry_(in_isr, handle_, received_data, timeout_ms);

        if (timeout_ms == 0U && receive_callback_ != nullptr) {
            return receive_callback_(in_isr, handle_, received_data);
        }

        return status;
    }
};

}  // namespace emdevif
