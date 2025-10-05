/**
 * @file peripheral_models_error_handler.cppm
 * @author DuYicheng
 * @date 2025-10-05
 * @brief 外设模型类的错误处理
 */

module;

#include <string_view>
#include <type_traits>

#include "emdevif/fatal_handler.hpp"

export module emdevif.peripheralModels.errorHandler;

import emdevif.errorHandler;

namespace emdevif::internal {

void ThisIsACompileTimeMessage_CouldNotFoundHandle() noexcept;

export constexpr void checkHandleIsExist(const void* handle, const std::string_view name) noexcept
{
    if (handle == nullptr) {
        if (std::is_constant_evaluated()) {
            ThisIsACompileTimeMessage_CouldNotFoundHandle();
            return;
        }

        using namespace std::literals;

        constexpr auto begin_str = "Could not find the serial handle named \""sv;
        constexpr auto end_str = "\"."sv;

        err_msg.clear();
        err_msg << begin_str << name << end_str;

        EMDEVIF_FATAL_HANDLER(err_msg.c_str());
        err_msg.clear();
    }
}

}  // namespace emdevif::internal

// module :private;

namespace emdevif::internal {

void ThisIsACompileTimeMessage_CouldNotFoundHandle() noexcept {}

}  // namespace emdevif::internal
