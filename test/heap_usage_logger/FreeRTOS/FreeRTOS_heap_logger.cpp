/**
 * @file FreeRTOS_heap_logger.cpp
 * @author DuYicheng
 * @date 2025-08-31
 * @brief FreeRTOS 堆内存使用记录
 */

#include <cstring>

#include "FreeRTOS.h"
#include "task.h"

#include "printf.h"

#include "emdevif_test_framework.h"

static char buffer[500];

extern "C" int nva_putchar(char)
{
    return 1;
}

void logSystemHeapUsage()
{
    HeapStats_t heap_stats;
    vPortGetHeapStats(&heap_stats);

    const char* const endl = emdevif_test___line_break_character___;

    int pos = 0;

    vTaskSuspendAll();
    /* clang-format off */

    pos += ::sprintf(&buffer[pos], "System heap usage:%s", endl);
    pos += ::sprintf(&buffer[pos], "xAvailableHeapSpaceInBytes      = %zu%s", heap_stats.xAvailableHeapSpaceInBytes, endl);
    pos += ::sprintf(&buffer[pos], "xSizeOfLargestFreeBlockInBytes  = %zu%s", heap_stats.xSizeOfLargestFreeBlockInBytes, endl);
    pos += ::sprintf(&buffer[pos], "xSizeOfSmallestFreeBlockInBytes = %zu%s", heap_stats.xSizeOfSmallestFreeBlockInBytes, endl);
    pos += ::sprintf(&buffer[pos], "xNumberOfFreeBlocks             = %zu%s", heap_stats.xNumberOfFreeBlocks, endl);
    pos += ::sprintf(&buffer[pos], "xMinimumEverFreeBytesRemaining  = %zu%s", heap_stats.xMinimumEverFreeBytesRemaining, endl);
    pos += ::sprintf(&buffer[pos], "xNumberOfSuccessfulAllocations  = %zu%s", heap_stats.xNumberOfSuccessfulAllocations, endl);
           ::sprintf(&buffer[pos], "xNumberOfSuccessfulFrees        = %zu", heap_stats.xNumberOfSuccessfulFrees);
    /* clang-format on */
    xTaskResumeAll();

    TEST_LOG("%s", buffer);
}
