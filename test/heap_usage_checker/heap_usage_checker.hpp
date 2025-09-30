/**
 * @file heap_usage_checker.hpp
 * @author DuYicheng
 * @date 2025-08-31
 * @brief 堆内存使用记录输出函数
 */

#pragma once
#ifndef EMDEVIF_TEST_HEAP_USAGE_CHECKER_HPP
#define EMDEVIF_TEST_HEAP_USAGE_CHECKER_HPP

#include <tuple>

void logSystemHeapUsage() noexcept;

std::tuple<std::size_t, std::size_t> getHeapAllocationInfo() noexcept;

#define CHECK_MEMORY_LEAK(void)                                                        \
    do {                                                                               \
        EMDEVIF_TEST_TEST_CASE_BEGIN(CheckMemoryLeak)                                  \
        {                                                                              \
            const auto [allocate_count, deallocate_count] = ::getHeapAllocationInfo(); \
            EMDEVIF_TEST_EXPECT_TRUE(allocate_count == deallocate_count)               \
                ->EMDEVIF_TEST_MESSAGE("allocate_count = %zu, deallocate_count = %zu", \
                                       allocate_count,                                 \
                                       deallocate_count);                              \
        }                                                                              \
        EMDEVIF_TEST_TEST_CASE_END();                                                  \
    } while (0)

#endif  // !EMDEVIF_TEST_HEAP_USAGE_CHECKER_HPP
