/**
 * @file can.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_MODEL_CAN_HPP
    #define EMDEVIF_PERIPHERAL_MODEL_CAN_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/core/error_handler.hpp"

        #include <cstdint>

        #include <span>
        #include <type_traits>
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

class CanModel
{
public:
    // IDE
    enum class DataIdentifier : uint_fast8_t {
        StandardId = 0,
        ExtendedId
    };

    // RTR
    enum class RemoteTransmissionRequest : uint_fast8_t {
        DataFrame = 0,
        RemoteFrame
    };

    using DataLength = uint_fast8_t;

    struct DataHeader {
        uint32_t id;
        DataIdentifier ide;
        RemoteTransmissionRequest rtr;
        DataLength dlc;
    };

    using ReceiveCallback = ErrorCode (*)(bool in_isr,
                                          void* handle,
                                          DataHeader& received_header,
                                          std::span<uint8_t> received_data);

    using TransmitFunction = ErrorCode (*)(bool in_isr,
                                           void* handle,
                                           const DataHeader& header,
                                           std::span<const uint8_t> data);

    struct Instance {
        void* const handle{nullptr};

        const ReceiveCallback receive_callback{nullptr};
        const TransmitFunction transmit_function{nullptr};
    };
};

static_assert(std::is_standard_layout_v<CanModel::Instance>);

}  // namespace emdevif

#endif  // !EMDEVIF_PERIPHERAL_MODEL_CAN_HPP
