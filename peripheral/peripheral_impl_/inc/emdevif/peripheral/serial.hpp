/**
 * @file serial.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_SERIAL_HPP
    #define EMDEVIF_PERIPHERAL_SERIAL_HPP

    #include "emdevif/core/detail/config.hpp"
    #include "emdevif/peripheral/detail/peripheral_error_handler.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/peripheral/peripheral_handle_map.hpp"
        #include "emdevif/peripheral/model/serial.hpp"
        #include "emdevif/core/error_handler.hpp"

        #include <cstdint>

        #include <limits>
        #include <string_view>
        #include <span>
        #include <type_traits>

        #include "emdevif/core/fatal_handler.h"
    #endif

EMDEVIF_MODULE_EXPORT
namespace emdevif {

class Serial : public SerialModel
{
private:
    SerialModel::Instance* instance_;

public:
    Serial() = delete;

    explicit constexpr Serial(const std::string_view name) noexcept
        : instance_(static_cast<SerialModel::Instance*>(PeripheralHandleMap::findHandle(name)))
    {
        detail::PeripheralErrorHandler::checkInstanceIsExist(instance_, "Serial", name);
    }

    SerialModel::State getStatus(const bool in_isr) const noexcept  // NOLINT(*-use-nodiscard)
    {
        EMDEVIF_ASSERT(instance_->get_state_function != nullptr);
        return instance_->get_state_function(in_isr, instance_->handle);
    }

    ErrorCode receive(const bool in_isr,  // NOLINT(*-use-nodiscard)
                      const std::span<uint8_t> received_data,
                      const uint32_t timeout_ms) const noexcept
    {
        EMDEVIF_ASSERT(instance_->receive_function != nullptr);
        return instance_->receive_function(in_isr, instance_->handle, received_data, timeout_ms);
    }

    ErrorCode receiveCallbackEntry(const bool in_isr,
                                   void* handle,
                                   const std::span<uint8_t> received_data) const noexcept
    {
        EMDEVIF_ASSERT(instance_->receive_callback != nullptr);
        return instance_->receive_callback(in_isr, handle, received_data);
    }

    ErrorCode transmit(const bool in_isr,  // NOLINT(*-use-nodiscard)
                       const std::span<const uint8_t> transmit_data,
                       const uint32_t timeout_ms) const noexcept
    {
        EMDEVIF_ASSERT(instance_->transmit_function != nullptr);
        const auto status = instance_->transmit_function(in_isr, instance_->handle, transmit_data, timeout_ms);
        return status;
    }

    static constexpr uint32_t max_delay = std::numeric_limits<uint32_t>::max();
};

}  // namespace emdevif

#endif  // !EMDEVIF_PERIPHERAL_SERIAL_HPP
