/**
 * @file error_handler.cppm
 * @brief 错误处理
 */

module;

#include "emdevif/core/error_handler.hpp"

export module emdevif.core.error_handler;

export namespace emdevif {
    using ::emdevif::TerminateFunction;
    using ::emdevif::terminate;
    using ::emdevif::registerTerminateFunction;
    using ::emdevif::FatalHandlerCallBack;
    using ::emdevif::fatalHandler;
    using ::emdevif::registerFatalHandler;
    using ::emdevif::AssertFailedHandler;
    using ::emdevif::emdevif_assert;
    using ::emdevif::registerAssertFailedHandler;
    using ::emdevif::ErrorCode;
    using ::emdevif::terminateIfNotSucceed;
    using ::emdevif::terminateIfNullptr;
    using ::emdevif::ErrorWithCodeException;
}
