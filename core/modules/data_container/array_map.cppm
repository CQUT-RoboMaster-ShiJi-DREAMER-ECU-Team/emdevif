/**
 * @file array_map.cppm
 * @brief 数组键值对（通过固定大小的数组实现的键值对容器）
 */

module;

#include "emdevif/core/data_container/array_map.hpp"

export module emdevif.core.data_container.array_map;

export namespace emdevif {
    using ::emdevif::ArrayMap;
    using ::emdevif::makeArrayMap;
    using ::emdevif::erase_if;
    using ::emdevif::StaticMap;
    using ::emdevif::makeStaticMap;
}
