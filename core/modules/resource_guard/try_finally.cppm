/**
 * @file try_finally.cppm
 * @brief try-finally 模式
 */

module;

#include "emdevif/core/resource_guard/try_finally.hpp"

export module emdevif.core.resource_guard.try_finally;

export namespace emdevif {
    using ::emdevif::ValidTryFunction;
    using ::emdevif::ValidFinallyFunction;
    using ::emdevif::tryFinally;
}
