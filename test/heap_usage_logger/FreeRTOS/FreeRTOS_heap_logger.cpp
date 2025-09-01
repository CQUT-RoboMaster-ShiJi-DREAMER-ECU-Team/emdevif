/**
 * @file FreeRTOS_heap_logger.cpp
 * @author DuYicheng
 * @date 2025-08-31
 * @brief FreeRTOS 堆内存使用记录
 */

#include <cstdio>
#include <cstring>

#include "nva/print.h"

#include "FreeRTOS.h"
#include "task.h"

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

    std::memset(buffer, '\0', sizeof buffer);

    vTaskSuspendAll();
    /* clang-format off */
    nva::format(buffer,
        "System(FreeRTOS) heap usage:{}"
        "  xAvailableHeapSpaceInBytes      = {}{}"
        "  xSizeOfLargestFreeBlockInBytes  = {}{}"
        "  xSizeOfSmallestFreeBlockInBytes = {}{}"
        "  xNumberOfFreeBlocks             = {}{}"
        "  xMinimumEverFreeBytesRemaining  = {}{}"
        "  xNumberOfSuccessfulAllocations  = {}{}"
        "  xNumberOfSuccessfulFrees        = {}",
        nva::add(endl,
        nva::add(heap_stats.xAvailableHeapSpaceInBytes, nva::add(endl,
        nva::add(heap_stats.xSizeOfLargestFreeBlockInBytes, nva::add(endl,
        nva::add(heap_stats.xSizeOfSmallestFreeBlockInBytes, nva::add(endl,
        nva::add(heap_stats.xNumberOfFreeBlocks, nva::add( endl,
        nva::add(heap_stats.xMinimumEverFreeBytesRemaining, nva::add(endl,
        nva::add(heap_stats.xNumberOfSuccessfulAllocations, nva::add(endl,
        nva::add(heap_stats.xNumberOfSuccessfulFrees, NVA_START)))))))))))))));

    // 由于未知原因，此处使用 sprintf 会进入 HardFault，故不使用。
    // pos += sprintf(&buffer[pos], "System heap usage:%s", endl);
    // pos += sprintf(&buffer[pos], "xAvailableHeapSpaceInBytes     =%zu%s", heap_stats.xAvailableHeapSpaceInBytes, endl);
    // pos += sprintf(&buffer[pos], "xSizeOfLargestFreeBlockInBytes =%zu%s", heap_stats.xSizeOfLargestFreeBlockInBytes, endl);
    // pos += sprintf(&buffer[pos], "xSizeOfSmallestFreeBlockInBytes=%zu%s", heap_stats.xSizeOfSmallestFreeBlockInBytes, endl);
    // pos += sprintf(&buffer[pos], "xNumberOfFreeBlocks            =%zu%s", heap_stats.xNumberOfFreeBlocks, endl);
    // pos += sprintf(&buffer[pos], "xMinimumEverFreeBytesRemaining =%zu%s", heap_stats.xMinimumEverFreeBytesRemaining, endl);
    // pos += sprintf(&buffer[pos], "xNumberOfSuccessfulAllocations =%zu%s", heap_stats.xNumberOfSuccessfulAllocations, endl);
    //        sprintf(&buffer[pos], "xNumberOfSuccessfulFrees       =%zu", heap_stats.xNumberOfSuccessfulFrees);
    /* clang-format on */
    xTaskResumeAll();

    TEST_LOG("%s", buffer);
}
