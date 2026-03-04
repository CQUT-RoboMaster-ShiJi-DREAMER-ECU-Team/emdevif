/**
 * @file config.hpp
 * @brief emdevif 配置
 */

#pragma once
#ifndef EMDEVIF_CORE_DETAIL_CONFIG_HPP
    #define EMDEVIF_CORE_DETAIL_CONFIG_HPP

    #ifndef EMDEVIF_USE_MODULES
        #define EMDEVIF_USE_MODULES 0
    #endif

    #ifdef EMDEVIF_MODULE_INTERFACE_UNIT
    /* clang-format off */
        #define EMDEVIF_MODULE_EXPORT       export
        #define EMDEVIF_MODULE_EXPORT_BEGIN export {
        #define EMDEVIF_MODULE_EXPORT_END   }
    /* clang-format on */
    #else  // !EMDEVIF_MODULE_INTERFACE_UNIT
        #define EMDEVIF_MODULE_EXPORT
        #define EMDEVIF_MODULE_EXPORT_BEGIN
        #define EMDEVIF_MODULE_EXPORT_END
    #endif  //! EMDEVIF_MODULE_INTERFACE_UNIT

#endif      // !EMDEVIF_CORE_DETAIL_CONFIG_HPP
