/**
 * @file peripheral_base_module.cppm
 * @brief 外设模型类的错误处理
 */

module;

#include <string_view>
#include <type_traits>

#include "emdevif/core/simplify_decl_macros.hpp"

#include "emdevif/core/fatal_handler.h"

export module emdevif.peripheral.base;

import emdevif.core.error_handler;

namespace emdevif::detail {

export class PeripheralErrorHandler
{
public:
    EMDEVIF_DECLARE_ZERO_INSTANCE(PeripheralErrorHandler);

    static constexpr void checkInstanceIsExist(const void* handle,
                                               const std::string_view peripheral_name,
                                               const std::string_view handle_name) noexcept
    {
        if (handle == nullptr) {
            if (std::is_constant_evaluated()) {
                // 通过 ill-formated code（在常量表达式求值上下文中调用非常量表达式函数）触发编译期错误信息
                ThisIsACompileTimeMessage_CouldNotFoundHandle();
                return;
            }

            EMDEVIF_FATAL_HANDLER("Could not find the %s handle named \"%s\"",
                                  peripheral_name.data(),
                                  handle_name.data());
        }
    }

private:
    static void ThisIsACompileTimeMessage_CouldNotFoundHandle() noexcept;
};

}  // namespace emdevif::detail

// module :private;

namespace emdevif::detail {

void PeripheralErrorHandler::ThisIsACompileTimeMessage_CouldNotFoundHandle() noexcept {}

}  // namespace emdevif::detail
