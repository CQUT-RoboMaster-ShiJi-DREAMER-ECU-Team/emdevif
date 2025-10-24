/**
 * @file SerialModel.cppm
 * @author DuYicheng
 * @date 2025-10-13
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
        void* const handle_{nullptr};

        const GetStateFunction get_state_function_{nullptr};

        const ReceiveFunction receive_function_{noReceive};
        const ReceiveCallback receive_callback_{nullptr};

        const TransmitFunction transmit_function_{noTransmit};
    };

private:
    static ErrorCode noTransmit(bool, void*, std::span<const uint8_t>, uint32_t) noexcept;

    static ErrorCode noReceive(bool, void*, std::span<uint8_t>, uint32_t) noexcept;
};

static_assert(std::is_standard_layout_v<SerialModel::Instance>);

}  // namespace emdevif

// 待编译器实现
// module :private;

namespace emdevif {

ErrorCode SerialModel::noTransmit(bool, void*, const std::span<const uint8_t>, uint32_t) noexcept
{
    return ErrorCode::NotImplemented;
}

ErrorCode SerialModel::noReceive(bool, void*, std::span<uint8_t>, uint32_t) noexcept
{
    return ErrorCode::NotImplemented;
}

}  // namespace emdevif
