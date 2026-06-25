/**
 * @file type_traits.cppm
 * @brief 类型特征（STL type_traits 头文件的扩展）
 */

module;

#include <type_traits>
#include <utility>
#include <tuple>
#include <array>

#include "emdevif/core/type_traits.hpp"

export module emdevif.core.type_traits;

export namespace emdevif {
using ::emdevif::aggregate_to_pair;
using ::emdevif::aggregate_to_tuple;
using ::emdevif::is_consteval;
using ::emdevif::is_same_tuple_style;
using ::emdevif::is_same_tuple_style_v;
using ::emdevif::is_tuple_like;
using ::emdevif::is_tuple_like_v;
using ::emdevif::is_tuple_style;
using ::emdevif::is_tuple_style_v;
using ::emdevif::tuple_to_aggregate;
}  // namespace emdevif
