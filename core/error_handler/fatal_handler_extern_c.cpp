/**
 * @file fatal_handler_extern_c.cpp
 * @author DuYicheng
 * @date 2025-10-16
 * @brief 致命错误 - C 语言 API
 */

#include "emdevif/fatal_handler.h"

#include "emdevif/attributes_and_useful_macros.h"

import emdevif.errorHandler;

EMDEVIF_EXTERN_C_BEGIN

EMDEVIF_NO_RETURN void emdevif_fatal_handler_(emdevif_FatalHandlerParam_ param)  // NOLINT
{
    if (param.message == nullptr) {
        param.message = "";
    }
    emdevif::fatalHandler(param.file, param.line, param.message);
}

void emdevif_assert_(emdevif_AssertFuncParam_ param)  // NOLINT
{
    if (param.message == nullptr) {
        param.message = "";
    }
    emdevif::emdevif_assert(param.condition,
                            param.file,
                            param.line,
                            param.func_name,
                            param.condition_name,
                            param.message);
}

EMDEVIF_EXTERN_C_END
