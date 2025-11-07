/**
 * @file esp_idf_heap_checker.cpp
 * @author DuYicheng
 * @date 2025-11-06
 * @brief ESP-IDF 堆内存使用记录
 */

#include "heap_usage_checker.hpp"

#include <cstring>

#include "esp_heap_caps.h"

#include "printf.h"

#include "emdevif_test_framework.h"

static char buffer[500];

void logSystemHeapUsage() noexcept
{
    multi_heap_info_t heap_info;
    heap_caps_get_info(&heap_info, MALLOC_CAP_8BIT);

    const char* const endl = emdevif_test___line_break_character___;

    int pos = 0;

    /* clang-format off */
    pos += ::sprintf(&buffer[pos], "System heap usage:%s", endl);
    pos += ::sprintf(&buffer[pos], "total_free_bytes      = %zu%s", heap_info.total_free_bytes, endl);
    pos += ::sprintf(&buffer[pos], "total_allocated_bytes = %zu%s", heap_info.total_allocated_bytes, endl);
    pos += ::sprintf(&buffer[pos], "largest_free_block    = %zu%s", heap_info.largest_free_block, endl);
    pos += ::sprintf(&buffer[pos], "minimum_free_bytes    = %zu%s", heap_info.minimum_free_bytes, endl);
    pos += ::sprintf(&buffer[pos], "allocated_blocks      = %zu%s", heap_info.allocated_blocks, endl);
    pos += ::sprintf(&buffer[pos], "free_blocks           = %zu%s", heap_info.free_blocks, endl);
           ::sprintf(&buffer[pos], "total_blocks          = %zu", heap_info.total_blocks);
    /* clang-format on */

    TEST_LOG("%s", buffer);
}

std::tuple<std::size_t, std::size_t> getHeapAllocationInfo() noexcept
{
    multi_heap_info_t heap_info;
    heap_caps_get_info(&heap_info, MALLOC_CAP_8BIT);

    return std::make_tuple(heap_info.allocated_blocks, heap_info.free_blocks);
}
