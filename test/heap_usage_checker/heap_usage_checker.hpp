/**
 * @file heap_usage_checker.hpp
 * @brief 堆内存使用记录输出函数
 */

#pragma once
#ifndef EMDEVIF_TEST_HEAP_USAGE_CHECKER_HPP
#define EMDEVIF_TEST_HEAP_USAGE_CHECKER_HPP

// ReSharper disable once CppUnusedIncludeDirective
#include "emdevif_test_framework-with_prefix.h"

#include <cstddef>

struct HeapUsageInfo {
    std::size_t allocated_count;
    std::size_t free_count;
};

void logSystemHeapUsage() noexcept;

HeapUsageInfo getHeapAllocationInfo() noexcept;

inline HeapUsageInfo begin_heap_usage_info{};

#define RECORD_MEMORY_INFO()                                                                                    \
    do {                                                                                                        \
        EMDEVIF_TEST_TEST_CASE_BEGIN(RecordMemoryInfo)                                                          \
        {                                                                                                       \
            begin_heap_usage_info = ::getHeapAllocationInfo();                                                  \
            EMDEVIF_TEST_ASSERT_TRUE(begin_heap_usage_info.allocated_count >= begin_heap_usage_info.free_count, \
                                     "allocated_count=%zu, free_count=%zu",                                     \
                                     begin_heap_usage_info.allocated_count,                                     \
                                     begin_heap_usage_info.free_count);                                         \
        }                                                                                                       \
        EMDEVIF_TEST_TEST_CASE_END();                                                                           \
    } while (0)

#define CHECK_MEMORY_LEAK()                                                                                      \
    do {                                                                                                         \
        EMDEVIF_TEST_TEST_CASE_BEGIN(CheckMemoryLeak)                                                            \
        {                                                                                                        \
            const auto [allocate_count, deallocate_count] = ::getHeapAllocationInfo();                           \
            EMDEVIF_TEST_ASSERT_TRUE(allocate_count >= deallocate_count,                                         \
                                     "allocate_count=%zu, deallocate_count=%zu",                                 \
                                     allocate_count,                                                             \
                                     deallocate_count);                                                          \
            EMDEVIF_TEST_EXPECT_TRUE((allocate_count - deallocate_count) ==                                      \
                                     (begin_heap_usage_info.allocated_count - begin_heap_usage_info.free_count)) \
                ->EMDEVIF_TEST_MESSAGE(                                                                          \
                    "(allocate_count - deallocate_count)=%zu, (begin_heap_usage_info.allocated_count - "         \
                    "begin_heap_usage_info.free_count)=%zu",                                                     \
                    (allocate_count - deallocate_count),                                                         \
                    (begin_heap_usage_info.allocated_count - begin_heap_usage_info.free_count));                 \
        }                                                                                                        \
        EMDEVIF_TEST_TEST_CASE_END();                                                                            \
    } while (0)

#endif  // !EMDEVIF_TEST_HEAP_USAGE_CHECKER_HPP
