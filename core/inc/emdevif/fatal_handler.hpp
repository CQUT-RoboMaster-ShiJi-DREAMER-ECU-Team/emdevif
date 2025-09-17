/**
 * @file fatal_handler.hpp
 * @author 杜以成
 * @date 2025-06-14
 * @brief 致命错误宏
 */

#pragma once
#ifndef EMDEVIF_FATAL_HANDLER_HPP
#define EMDEVIF_FATAL_HANDLER_HPP

/**
 * emdevif 致命错误处理宏
 * @attention 需要导入模块 emdevif.error_handler 才能正常使用这个宏
 * @param ... 错误信息字符串（可以省略不写）
 */
#define EMDEVIF_FATAL_HANDLER(...) ::emdevif::fatalHandler(__FILE__, __LINE__ __VA_OPT__(, ) __VA_ARGS__)

/**
 * @def EMDEVIF_ASSERT(condition, ...)
 *
 * @brief emdevif 断言宏
 * @param condition 断言条件
 * @param ... 断言失败时的错误信息字符串（可以省略不写）
 * @attention 如果 NDEBUG 宏被定义，则此宏不进行任何操作
 */
#if (defined(NDEBUG))
#define EMDEVIF_ASSERT(condition, ...) ((void)0)
#else
#define EMDEVIF_ASSERT(condition, ...) \
    ::emdevif::emdevif_assert((condition), __FILE__, __LINE__, __func__, #condition __VA_OPT__(, ) __VA_ARGS__)
#endif

#endif  // !EMDEVIF_FATAL_HANDLER_HPP
