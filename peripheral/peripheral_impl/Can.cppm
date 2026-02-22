/**
 * @file Can.cppm
 * @brief CAN 通信
 */

module;

#include <cstdint>

#include <span>
#include <string_view>

#include "emdevif/core/fatal_handler.h"

export module emdevif.peripheral.can;

export import emdevif.core.error_handler;
import emdevif.peripheralHandleMap;
import emdevif.peripheral.base;
import emdevif.peripheral.model.can;

export namespace emdevif {

class Can : public CanModel
{
private:
    CanModel::Instance* const instance_;

public:
    Can() = delete;

    explicit constexpr Can(const std::string_view name) noexcept
        : instance_(static_cast<CanModel::Instance*>(PeripheralHandleMap::findHandle(name)))
    {
        detail::PeripheralErrorHandler::checkInstanceIsExist(instance_, "CAN", name);
    }

    ErrorCode receiveCallbackEntry(const bool in_isr,
                                   void* handle,
                                   DataHeader& received_header,
                                   const std::span<uint8_t> received_data) const noexcept
    {
        EMDEVIF_ASSERT(instance_->receive_callback != nullptr);
        return instance_->receive_callback(in_isr, handle, received_header, received_data);
    }

    // NOLINTNEXTLINE
    ErrorCode transmit(const bool in_isr, const DataHeader& header, const std::span<const uint8_t> data) const noexcept
    {
        EMDEVIF_ASSERT(instance_->transmit_function != nullptr);
        return instance_->transmit_function(in_isr, instance_->handle, header, data);
    }
};

}  // namespace emdevif
