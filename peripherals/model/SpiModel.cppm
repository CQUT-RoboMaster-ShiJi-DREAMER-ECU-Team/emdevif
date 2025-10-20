/**
 * @file SpiModel.cppm
 * @author DuYicheng
 * @date 2025-10-13
 * @brief SPI 通信模型
 */

module;

#include <cstdint>

#include <span>
#include <type_traits>

export module emdevif.peripheral.model.spi;

export import emdevif.errorHandler;

export namespace emdevif {

class SpiModel
{
public:
    using TransmitReceiveFunction = ErrorCode (*)(bool in_isr,
                                                  void* handle,
                                                  std::span<const uint8_t> tx_data,
                                                  std::span<uint8_t> rx_data,
                                                  uint32_t timeout_ms);

    struct Instance {
        void* const handle_;

        const TransmitReceiveFunction transmit_receive_function_;
    };
};

static_assert(std::is_standard_layout_v<SpiModel::Instance>);

}  // namespace emdevif
