/**
 * @file thread_constant_interface.cppm
 * @module emdevif.thread.constant:interface
 * @author 杜以成
 * @date 2025-06-18
 * @brief 线程相关常数接口
 */

module;

export module emdevif.thread.constant:interface;

export namespace emdevif::thread {

/**
 * 等待时间最大值
 */
consteval auto MAX_DELAY() noexcept;

}  // namespace emdevif::thread
