/**
 * @file test_main.cpp
 * @author DuYicheng
 * @date 2025-07-30
 * @brief 测试主函数
 */

#include <array>
#include <utility>
#include <string_view>

#include "test_main.hpp"

#include "heap_usage_checker.hpp"

#ifndef EMDEVIF_TEST_IGNORE_NAME_LIST
#error \
    "Please define the macro `EMDEVIF_TEST_IGNORE_NAME_LIST'. If nothing to ignore, define it to {""} ."
#endif

#define TEST_LIST_ARG(func_name) #func_name, func_name

static constexpr auto emdevif_test_list =
    std::to_array<std::pair<std::string_view, void (*)()>>({{TEST_LIST_ARG(cFeatureTest)},
                                                            {TEST_LIST_ARG(endianTest)},
                                                            {TEST_LIST_ARG(typeTraitsTest)},
                                                            {TEST_LIST_ARG(logSystemHeapUsage)},
                                                            {TEST_LIST_ARG(initOnceTest)},
                                                            {TEST_LIST_ARG(bitIntTest)},
                                                            {TEST_LIST_ARG(ringBufferTest)},
                                                            {TEST_LIST_ARG(loggerTest)},
                                                            {TEST_LIST_ARG(heapTest)},
                                                            {TEST_LIST_ARG(sysQueueTest)},
                                                            {TEST_LIST_ARG(threadAndMutexTest)},
                                                            {TEST_LIST_ARG(logSystemHeapUsage)}});

static constexpr auto emdevif_test_ignore_name_list = std::to_array<std::string_view>(EMDEVIF_TEST_IGNORE_NAME_LIST);

static constexpr bool inIgnoreNameList(const std::string_view& name) noexcept
{
    for (const auto& e : emdevif_test_ignore_name_list) {
        if (e == name) {
            return true;
        }
    }

    return false;
}

extern "C" void emdevif_testEntry(void)
{
    for (const auto& [test_name, test_func] : emdevif_test_list) {
        if (!inIgnoreNameList(test_name)) {
            test_func();
        }
    }
}
