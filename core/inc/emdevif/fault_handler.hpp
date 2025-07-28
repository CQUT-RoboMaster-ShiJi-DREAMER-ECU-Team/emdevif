/**
 * @file fault_handler.hpp
 * @author 杜以成
 * @date 2025-06-14
 * @brief 致命错误宏
 */

#pragma once
#ifndef EMDEVIF_FAULT_HANDLER_HPP
#define EMDEVIF_FAULT_HANDLER_HPP

/**
 * emdevif 致命错误处理宏
 * @attention 需要导入模块 emdevif.error_handler 才能正常使用这个宏
 * @param ... 错误信息字符串（可以省略不写）
 */
#define EMDEVIF_FAULT_HANDLER(...) faultHandler(__FILE__, __LINE__, ##__VA_ARGS__)

#endif // !EMDEVIF_FAULT_HANDLER_HPP
