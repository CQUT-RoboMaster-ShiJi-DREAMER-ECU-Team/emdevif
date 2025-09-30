/**
 * @file test_main.cpp
 * @author DuYicheng
 * @date 2025-07-30
 * @brief 测试主函数
 */

#include "test_main.hpp"

#include "heap_usage_checker.hpp"

extern "C" void emdevif_testEntry(void)
{
    logSystemHeapUsage();

    initOnceTest();
    bitIntTest();
    ringBufferTest();

    loggerTest();

    heapTest();
    threadAndMutexTest();

    logSystemHeapUsage();
}
