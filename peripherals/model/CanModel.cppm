/**
 * @file CanModel.cppm
 * @module emdevif.peripheral.model.can
 * @author DuYicheng
 * @date 2025-10-13
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
        void* const handle_{nullptr};

        const ReceiveCallback receive_callback_{noReceive};
        const TransmitFunction transmit_function_{noTransmit};
    };

private:
    static ErrorCode noTransmit(bool, void*, const DataHeader&, std::span<const uint8_t>) noexcept;

    static ErrorCode noReceive(bool, void*, DataHeader&, std::span<uint8_t>) noexcept;
};

static_assert(std::is_standard_layout_v<CanModel::Instance>);

}  // namespace emdevif

// 待编译器实现
// module :private;

namespace emdevif {

ErrorCode CanModel::noTransmit(bool, void*, const DataHeader&, std::span<const uint8_t>) noexcept
{
    return ErrorCode::NotImplemented;
}

ErrorCode CanModel::noReceive(bool, void*, DataHeader&, std::span<uint8_t>) noexcept
{
    return ErrorCode::NotImplemented;
}

}  // namespace emdevif
