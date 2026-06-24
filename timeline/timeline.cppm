/**
 * @file timeline.cppm
 * @brief 时间戳
 */

module;

#ifndef EMDEVIF_USER_DECLARES_PROVIDE_MODULE
    #include "emdevif/user_declares.hpp"
#endif

#include "emdevif/core/simplify_decl_macros.hpp"
#include "emdevif/timeline.hpp"

export module emdevif.timeline;

export namespace emdevif {
using ::emdevif::BaseTimePoint;
using ::emdevif::Timeline;
using ::emdevif::Duration;
}
