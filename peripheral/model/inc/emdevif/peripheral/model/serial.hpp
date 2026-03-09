/**
 * @file serial.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_MODEL_SERIAL_HPP
    #define EMDEVIF_PERIPHERAL_MODEL_SERIAL_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/core/error_handler.hpp"

        #include <cstdint>

        #include <span>
        #include <type_traits>
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

class SerialModel
{
public:
    enum State : uint_fast8_t {
        Ready = 0,
        Busy
    };

    using GetStateFunction = State (*)(bool in_isr, void* handle);

    using ReceiveFunction = ErrorCode (*)(bool in_isr,
                                          void* handle,
                                          std::span<uint8_t> received_data,
                                          uint32_t timeout_ms);

    using ReceiveCallback = ErrorCode (*)(bool in_isr, void* handle, std::span<uint8_t> received_data);

    using TransmitFunction = ErrorCode (*)(bool in_isr,
                                           void* handle,
                                           std::span<const uint8_t> data,
                                           uint32_t timeout_ms);

    struct Instance {
        void* const handle{nullptr};

        const GetStateFunction get_state_function{nullptr};

        const ReceiveFunction receive_function{nullptr};
        const ReceiveCallback receive_callback{nullptr};

        const TransmitFunction transmit_function{nullptr};
    };
};

static_assert(std::is_standard_layout_v<SerialModel::Instance>);

}  // namespace emdevif

#endif  // !EMDEVIF_PERIPHERAL_MODEL_SERIAL_HPP
