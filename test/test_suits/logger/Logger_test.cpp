/**
 * @file Logger_test.cpp
 * @author DuYicheng
 * @date 2025-09-19
 * @brief 日志测试
 */

#include "emdevif_test_framework.h"

#include "emdevif/logger.hpp"

import emdevif.logger;
import emdevif.userDeclares;
import emdevif.sys.thread;

#define EMDEVIF_LOGGER_TEST_FILE_NAME "Logger_test.cpp"

// #define EMDEVIF_LOGGER_TEST_DEVELOPER_CHECK_MODE

#ifdef EMDEVIF_LOGGER_TEST_DEVELOPER_CHECK_MODE
#define EMDEVIF_LOGGER_TEST_LOG(log_level, format, ...)              \
    do {                                                             \
        EMDEVIF_LOG(log_level, format __VA_OPT__(, ) __VA_ARGS__);   \
        TEST_LOG("%s", emdevif::user_declares::logger::getBuffer()); \
    } while (0)
#else
#define EMDEVIF_LOGGER_TEST_LOG(log_level, format, ...)            \
    do {                                                           \
        EMDEVIF_LOG(log_level, format __VA_OPT__(, ) __VA_ARGS__); \
    } while (0)
#endif

TEST_SUIT(LoggerTest)
{
    TEST_CASE_BEGIN(BasicTest)
    {
#line 1 EMDEVIF_LOGGER_TEST_FILE_NAME
        EMDEVIF_LOGGER_TEST_LOG(emdevif::LogLevel::Info, "An Info Message.");

#line 1437 EMDEVIF_LOGGER_TEST_FILE_NAME
        EMDEVIF_LOGGER_TEST_LOG(emdevif::LogLevel::Warning, "An Warning Message with integer num %d.", 114514);

        emdevif::Thread::delay(emdevif::Thread::msToTick(2));

        constexpr auto expect_log_out =
            "0          [Info] An Info Message. (at Logger_test.cpp:1 in function `emdevif_test__LoggerTest__')\r\n"
            "1          [Warning] An Warning Message with integer num 114514. (at Logger_test.cpp:1437 in function "
            "`emdevif_test__LoggerTest__')\r\n";
        EXPECT_STREQ(::emdevif::user_declares::logger::getBuffer(), expect_log_out);
    }
    TEST_CASE_END();
}

void loggerTest()
{
    RUN_SUIT(LoggerTest);
}
