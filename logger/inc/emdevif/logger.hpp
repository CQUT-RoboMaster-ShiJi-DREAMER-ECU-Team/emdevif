/**
 * @file logger.hpp
 * @author DuYicheng
 * @date 2025-09-18
 * @brief 日志
 */

#pragma once
#ifndef EMDEVIF_LOGGER_HPP
#define EMDEVIF_LOGGER_HPP

/**
 * 输出日志信息
 * @param level 日志等级。值可以从 @ref emdevif::LogLevel 中选择
 * @param format 日志信息的格式化字符串
 * @param ... 可变参数列表，用于填充格式化字符串中的占位符
 */
#define EMDEVIF_LOG(level, format, ...) \
    ::emdevif::Logger::getInstance().log(__FILE__, __LINE__, __func__, level, format __VA_OPT__(, ) __VA_ARGS__)

/**
 * @def EMDEVIF_DLOG(level, format, ...)
 *
 * 在调试模式下（未定义 NDEBUG 宏的时候）输出日志信息，在其他模式下不进行任何操作。
 * @param level 日志等级。值可以从 @ref emdevif::LogLevel 中选择
 * @param format 日志信息的格式化字符串
 * @param ... 可变参数列表，用于填充格式化字符串中的占位符
 */
#ifdef NDEBUG
#define EMDEVIF_DLOG(level, format, ...) ((void)0)
#else
#define EMDEVIF_DLOG(level, format, ...) EMDEVIF_LOG(level, format __VA_OPT__(, ) __VA_ARGS__)
#endif

#endif  // !EMDEVIF_LOGGER_HPP
