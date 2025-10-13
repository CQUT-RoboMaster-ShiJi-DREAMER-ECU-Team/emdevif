/**
 * @file Can.cppm
 * @module emdevif.peripheral.can
 * @author DuYicheng
 * @date 2025-09-12
 * @brief CAN 通信
 */

module;

#include <cstdint>

#include <span>
#include <string_view>

#include "emdevif/fatal_handler.hpp"

export module emdevif.peripheral.can;

export import emdevif.errorHandler;
import emdevif.peripheralHandleMap;
import emdevif.peripheralModels.errorHandler;
import emdevif.peripherals.models.can;

export namespace emdevif {

class Can : public CanModel
{
private:
    CanModel::Instance* const instance_;

public:
    Can() = delete;

    explicit constexpr Can(const std::string_view name) noexcept
        : instance_(static_cast<CanModel::Instance*>(PeripheralHandleMap::findHandle(name).value_or(nullptr)))
    {
        internal::checkHandleIsExist(instance_, "CAN");
    }

    ErrorCode receiveCallbackEntry(const bool in_isr,
                                   void* handle,
                                   DataHeader& received_header,
                                   const std::span<uint8_t> received_data) const noexcept
    {
        EMDEVIF_ASSERT(instance_->receive_callback_ != nullptr);
        return instance_->receive_callback_(in_isr, handle, received_header, received_data);
    }

    // NOLINTNEXTLINE
    ErrorCode transmit(const bool in_isr, const DataHeader& header, const std::span<const uint8_t> data) const noexcept
    {
        EMDEVIF_ASSERT(instance_->transmit_function_ != nullptr);
        return instance_->transmit_function_(in_isr, instance_->handle_, header, data);
    }
};

}  // namespace emdevif
