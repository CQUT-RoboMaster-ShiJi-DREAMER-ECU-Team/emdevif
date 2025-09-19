/**
 * @file logger.hpp
 * @author DuYicheng
 * @date 2025-09-18
 * @brief 日志
 */

#pragma once
#ifndef EMDEVIF_LOGGER_HPP
#define EMDEVIF_LOGGER_HPP

#define EMDEVIF_LOG(level, format, ...) \
    ::emdevif::Logger::getInstance().log(__FILE__, __LINE__, __func__, level, format __VA_OPT__(, ) __VA_ARGS__)

#ifdef NDEBUG
#define EMDEVIF_DLOG(level, format, ...) ((void)0)
#else
#define EMDEVIF_DLOG(level, format, ...) EMDEVIF_LOG(level, format __VA_OPT__(, ) __VA_ARGS__)
#endif

#endif  // !EMDEVIF_LOGGER_HPP
