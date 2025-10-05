/**
 * @file Can.cppm
 * @module emdevif.peripheralModels.can
 * @author DuYicheng
 * @date 2025-09-12
 * @brief CAN 通信
 */

module;

#include <cstdint>

#include <span>
#include <string_view>

#include "emdevif/fatal_handler.hpp"

export module emdevif.peripheralModels.can;

export import emdevif.errorHandler;
import emdevif.peripheralHandleMap;
import emdevif.peripheralModels.errorHandler;

export namespace emdevif {

class Can
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

    using ReceiveCallback = ErrorCode (&)(bool in_isr,
                                          void* handle,
                                          DataHeader& received_header,
                                          std::span<uint8_t> received_data);

    using TransmitFunction = ErrorCode (&)(bool in_isr,
                                           void* handle,
                                           const DataHeader& header,
                                           std::span<const uint8_t> data);

private:
    void* handle_;

    ReceiveCallback receive_callback_;
    TransmitFunction transmit_function_;

public:
    Can() = delete;

    constexpr Can(const std::string_view name,
                  const ReceiveCallback receive_callback = noReceive,
                  const TransmitFunction transmit_function = noTransmit) noexcept
        : handle_(PeripheralHandleMap::findHandle(name).value_or(nullptr)),
          receive_callback_(receive_callback),
          transmit_function_(transmit_function)
    {
        internal::checkHandleIsExist(handle_, "CAN");
    }

    ErrorCode receiveCallbackEntry(const bool in_isr,
                                   void* handle,
                                   DataHeader& received_header,
                                   std::span<uint8_t> received_data) const noexcept
    {
        return receive_callback_(in_isr, handle, received_header, received_data);
    }

    ErrorCode transmit(const bool in_isr,
                       void* handle,
                       const DataHeader& header,
                       std::span<const uint8_t> data) const noexcept
    {
        return transmit_function_(in_isr, handle, header, data);
    }

private:
    static ErrorCode noTransmit(bool, void*, const DataHeader&, std::span<const uint8_t>) noexcept;

    static ErrorCode noReceive(bool, void*, DataHeader&, std::span<uint8_t>) noexcept;
};

}  // namespace emdevif

// 待编译器实现
// module :private;

namespace emdevif {

ErrorCode Can::noTransmit(bool, void*, const DataHeader&, std::span<const uint8_t>) noexcept
{
    return ErrorCode::NotImplemented;
}

ErrorCode Can::noReceive(bool, void*, DataHeader&, std::span<uint8_t>) noexcept
{
    return ErrorCode::NotImplemented;
}

}  // namespace emdevif
