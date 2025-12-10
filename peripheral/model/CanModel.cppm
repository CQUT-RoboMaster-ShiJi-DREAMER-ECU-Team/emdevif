/**
 * @file CanModel.cppm
 * @brief CAN 通信模型
 */

module;

#include <cstdint>

#include <span>
#include <type_traits>

export module emdevif.peripheral.model.can;

export import emdevif.errorHandler;

export namespace emdevif {

class CanModel
{
public:
    enum class DataIdentifier : uint_fast8_t {
        StandardId = 0,
        ExtendedId
    };
    using Ide = DataIdentifier;

    enum class RemoteTransmissionRequest : uint_fast8_t {
        DataFrame = 0,
        RemoteFrame
    };
    using Rtr = RemoteTransmissionRequest;

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
