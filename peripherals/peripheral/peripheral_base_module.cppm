/**
 * @file peripheral_base_module.cppm
 * @module emdevif.peripheral.errorHandler
 * @author DuYicheng
 * @date 2025-10-05
 * @brief 外设模型类的错误处理
 */

module;

#include <string_view>
#include <type_traits>

#include "emdevif/simplify_decl_macros.hpp"

#include "emdevif/fatal_handler.h"

export module emdevif.peripheral.base;

import emdevif.errorHandler;

namespace emdevif::internal {

export class PeripheralErrorHandler
{
public:
    EMDEVIF_DECLARE_ZERO_INSTANCE(PeripheralErrorHandler);

    static constexpr void checkInstanceIsExist(const void* handle, const std::string_view name) noexcept
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

private:
    static void ThisIsACompileTimeMessage_CouldNotFoundHandle() noexcept;
};

}  // namespace emdevif::internal

// module :private;

namespace emdevif::internal {

void PeripheralErrorHandler::ThisIsACompileTimeMessage_CouldNotFoundHandle() noexcept {}

}  // namespace emdevif::internal
