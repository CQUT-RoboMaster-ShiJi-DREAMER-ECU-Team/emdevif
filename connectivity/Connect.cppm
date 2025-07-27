/**
 * @file Connect.cppm
 * @module rmdev.connectivity.Connect
 * @author 杜以成
 * @date 2025-05-09
 * @brief 通信基类
 */

module;

#include <cstdint>
#include <string_view>

export module rmdev.connectivity.Connect;

import rmdev.util.InitOnce;

export namespace rmdev {

/**
 * 通信方式
 */
enum class ConnectMethod : std::uint8_t {
    BLOCK = 0U,  ///< 阻塞式
    INTERRUPT,   ///< 中断式
    DMA          ///< DMA 模式
};

/**
 * 通信基类
 */
class Connect
{
public:
    Connect() = delete;

    /**
     * 通信类构造函数
     * @param name 端口名称
     * @param tx 发送方式
     * @param rx 接收方式
     */
    Connect(const std::string_view name, const ConnectMethod tx, const ConnectMethod rx)
        : port_name(name), tx_method(tx), rx_method(rx)
    {
    }

protected:
    const std::string_view port_name;  ///< 端口名称
    const ConnectMethod tx_method{};   ///< 发送方式
    const ConnectMethod rx_method{};   ///< 接收方式
    InitOnce<void*> handle{};          ///< 句柄
};

}  // namespace rmdev
