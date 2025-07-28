/**
 * @file thread_constant_implements.cppm
 * @module emdevif.thread.constant:implements
 * @author 杜以成
 * @date 2025-06-18
 * @brief 线程相关常数实现
 */

module;

#include "FreeRTOS.h"

export module emdevif.thread.constant:implements;

export namespace emdevif::thread {

consteval auto MAX_DELAY() noexcept
{
    return portMAX_DELAY;
}

}  // namespace emdevif::thread
