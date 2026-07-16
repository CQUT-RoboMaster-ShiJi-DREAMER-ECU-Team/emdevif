/**
 * @file inplace_dynamic_array.cppm
 * @brief 固定容量的原地动态数组模块接口
 */

module;

#include "emdevif/core/data_container/inplace_dynamic_array.hpp"

export module emdevif.core.data_container.inplace_dynamic_array;

export namespace emdevif {
using ::emdevif::erase;
using ::emdevif::eraseIf;
using ::emdevif::InplaceDynamicArray;
using ::emdevif::swap;
}  // namespace emdevif
