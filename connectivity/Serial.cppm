/**
 * @file Serial.cppm
 * @module rmdev.connectivity.Serial
 * @author 杜以成
 * @date 2025-05-09
 * @brief 串口通信
 */

module;

#include <string_view>
#include <cstdint>
#include "rmdev/simplify_decl_macros.hpp"

export module rmdev.connectivity.Serial;

export import rmdev.error_handler;

import rmdev.connectivity.Connect;

export namespace rmdev {

class Serial : public Connect
{
public:
    Serial() = delete;
    RMDEV_DELETE_COPY_AND_MOVE(Serial);

    Serial(const std::string_view name, const ConnectMethod tx, const ConnectMethod rx) : Connect(name, tx, rx) {}

    ErrorCode tx(const uint8_t* data, std::size_t size) const noexcept;

    ErrorCode rx(const uint8_t* data, std::size_t size) const noexcept;
};

}  // namespace rmdev
