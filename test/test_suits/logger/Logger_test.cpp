/**
 * @file Logger_test.cpp
 * @brief 日志测试
 */

#include "emdevif_test_framework.h"

#include <cstdint>

#include "emdevif/core/line_separator.h"
#include "emdevif/logger.hpp"

import emdevif.logger;
import emdevif.userDeclares;
import emdevif.sys.thread;

#define EMDEVIF_LOGGER_TEST_FILE_NAME "Logger_test.cpp"

// #define EMDEVIF_LOGGER_TEST_DEVELOPER_CHECK_MODE

TEST_SUIT(LoggerTest)
{
    TEST_CASE_BEGIN(BasicTest)
    {
#line 1 EMDEVIF_LOGGER_TEST_FILE_NAME
        EMDEVIF_LOG(emdevif::LogLevel::Info, "An Info Message.");

#line 1437 EMDEVIF_LOGGER_TEST_FILE_NAME
        EMDEVIF_LOG(emdevif::LogLevel::Warning, "An Warning Message with integer num %d.", 114514);

#line 65535 EMDEVIF_LOGGER_TEST_FILE_NAME
        EMDEVIF_LOG(emdevif::LogLevel::Debug, "Hello, %s", "Logger");

        emdevif::Thread::delay(emdevif::Thread::msToTick(2));

#ifdef EMDEVIF_LOGGER_TEST_DEVELOPER_CHECK_MODE
        TEST_LOG("%s", emdevif::user_declares::logger::getBuffer());
#else
        constexpr auto expect_log_out =
            "0          [Info] An Info Message. (at Logger_test.cpp:1 in function "
            "`emdevif_test__LoggerTest__')" EMDEVIF_LINE_SEPARATOR
            "1          [Warning] An Warning Message with integer num 114514. (at Logger_test.cpp:1437 in function "
            "`emdevif_test__LoggerTest__')" EMDEVIF_LINE_SEPARATOR
            "2          [Debug] Hello, Logger (at Logger_test.cpp:65535 in function "
            "`emdevif_test__LoggerTest__')" EMDEVIF_LINE_SEPARATOR;
        EXPECT_STREQ(::emdevif::user_declares::logger::getBuffer(), expect_log_out);
#endif
    }
    TEST_CASE_END();
}

void loggerTest()
{
    RUN_SUIT(LoggerTest);
}
