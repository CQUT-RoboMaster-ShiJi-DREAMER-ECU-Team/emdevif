/**
 * @file can.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_CAN_HPP
    #define EMDEVIF_PERIPHERAL_CAN_HPP

    #include "emdevif/core/detail/config.hpp"
    #include "emdevif/peripheral/detail/peripheral_error_handler.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/peripheral/peripheral_handle_map.hpp"
        #include "emdevif/peripheral/model/can.hpp"
        #include "emdevif/core/error_handler.hpp"

        #include <cstdint>

        #include <span>
        #include <string_view>

        #include "emdevif/core/fatal_handler.h"
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

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

#endif  // !EMDEVIF_PERIPHERAL_CAN_HPP
