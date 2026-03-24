/**
 * @file Logger_test.cpp
 * @brief 日志测试
 */

#include "emdevif_test_framework.h"

#include <cstdint>

#include "emdevif/core/line_separator.h"

#ifdef EMDEVIF_USE_MODULES
import emdevif.logger;
import emdevif.user_declares;
import emdevif.system.thread;
import emdevif.core.integer_suffix;
#else
    #include "emdevif/logger.hpp"
    #include "emdevif/user_declares.hpp"
    #include "emdevif/system/thread.hpp"
    #include "emdevif/core/integer_suffix.hpp"
#endif

// #define EMDEVIF_LOGGER_TEST_DEVELOPER_CHECK_MODE

TEST_SUIT(LoggerTest)
{
    TEST_CASE_BEGIN(BasicTest)
    {
        using namespace emdevif;

        logger::info("Hello, emdevif_logger! This is an Info Message.");
        logger::info("Hi, %s", "emdevif_logger lib!");
        logger::debug("let's test an integer: %d", 114514);
        logger::warning("An Warning Message with integer num %d.", 1919810);
        logger::error("An Error Message with a ptrdiff_t value %td.", -1437_ptrdiff);
        logger::info("gaoo; %chiqwf%%hfiwl" EMDEVIF_LINE_SEPARATOR "hf%sivb%dihiiiu", 't', "fsegnn", 35673);

        emdevif::Thread::delay(emdevif::Thread::msToTick(2));

#ifdef EMDEVIF_LOGGER_TEST_DEVELOPER_CHECK_MODE
        TEST_LOG("%s", emdevif::user_declares::logger::getBuffer());
#else
        constexpr auto expect_log_out =
            "0             Info Hello, emdevif_logger! This is an Info Message." EMDEVIF_LINE_SEPARATOR
            "1             Info Hi, emdevif_logger lib!" EMDEVIF_LINE_SEPARATOR
            "2          Warning An Warning Message with integer num 1919810." EMDEVIF_LINE_SEPARATOR
            "3            Error An Error Message with a ptrdiff_t value -1437." EMDEVIF_LINE_SEPARATOR
            "4             Info gaoo; thiqwf%hfiwl" EMDEVIF_LINE_SEPARATOR
            "hffsegnnivb35673ihiiiu" EMDEVIF_LINE_SEPARATOR;
        EXPECT_STREQ(::emdevif::user_declares::logger::getBuffer(), expect_log_out);
#endif
    }
    TEST_CASE_END();
}

void loggerTest()
{
    RUN_SUIT(LoggerTest);
}
