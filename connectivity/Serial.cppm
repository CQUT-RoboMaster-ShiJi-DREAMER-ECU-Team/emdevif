/**
 * @file Serial.cppm
 * @module emdevif.connectivity.Serial
 * @author 杜以成
 * @date 2025-05-09
 * @brief 串口通信
 */

module;

#include <string_view>
#include <cstdint>
#include "emdevif/simplify_decl_macros.hpp"

export module emdevif.connectivity.Serial;

export import emdevif.error_handler;

import emdevif.connectivity.Connect;

export namespace emdevif {

class Serial : public Connect
{
public:
    Serial() = delete;
    EMDEVIF_DELETE_COPY_AND_MOVE(Serial);

    Serial(const std::string_view name, const ConnectMethod tx, const ConnectMethod rx) : Connect(name, tx, rx) {}

    ErrorCode tx(const uint8_t* data, std::size_t size) const noexcept;

    ErrorCode rx(const uint8_t* data, std::size_t size) const noexcept;
};

}  // namespace emdevif
