/**
 * @file peripheral_error_handler.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_PERIPHERAL_DETAIL_PERIPHERAL_ERROR_HANDLER_HPP
    #define EMDEVIF_PERIPHERAL_DETAIL_PERIPHERAL_ERROR_HANDLER_HPP

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include <string_view>
        #include <type_traits>

        #include "emdevif/core/simplify_decl_macros.hpp"
        #include "emdevif/core/error_handler.hpp"

        #include "emdevif/core/fatal_handler.h"
    #endif

namespace emdevif::detail {

class PeripheralErrorHandler
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
    static void ThisIsACompileTimeMessage_CouldNotFoundHandle() noexcept {}
};

}  // namespace emdevif::detail

#endif  // !EMDEVIF_PERIPHERAL_DETAIL_PERIPHERAL_ERROR_HANDLER_HPP
