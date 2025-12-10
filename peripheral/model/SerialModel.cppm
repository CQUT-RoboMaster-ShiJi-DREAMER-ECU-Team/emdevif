/**
 * @file SerialModel.cppm
 * @brief 串口模型
 */

module;

#include <cstdint>

#include <type_traits>
#include <span>

export module emdevif.peripheral.model.serial;

export import emdevif.errorHandler;

export namespace emdevif {

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
