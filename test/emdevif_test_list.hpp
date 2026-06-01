/**
 * @file emdevif_test_list.hpp
 * @brief
 */

#pragma once
#ifndef EMDEVIF_TEST_LIST_HPP
    #define EMDEVIF_TEST_LIST_HPP

    #include <array>
    #include <utility>
    #include <string_view>

    #include "test_main.hpp"

    #include "heap_usage_checker.hpp"

    #define TEST_LIST_ARG(func_name) #func_name, func_name

    #ifndef USE_GENERATED_EMDEVIF_TEST_LIST

static constexpr auto emdevif_test_list =
    std::to_array<std::pair<std::string_view, void (*)()>>({{TEST_LIST_ARG(cFeatureTest)},
                                                            {TEST_LIST_ARG(endianTest)},
                                                            {TEST_LIST_ARG(typeTraitsTest)},
                                                            {TEST_LIST_ARG(resourceGuardTest)},
                                                            {TEST_LIST_ARG(initOnceTest)},
                                                            {TEST_LIST_ARG(bitIntTest)},
                                                            {TEST_LIST_ARG(mapTest)},
                                                            {TEST_LIST_ARG(ringBufferTest)},
                                                            {TEST_LIST_ARG(loggerTest)},
                                                            {TEST_LIST_ARG(heapTest)},
                                                            {TEST_LIST_ARG(sysQueueTest)},
                                                            {TEST_LIST_ARG(sysMessageSlotTest)},
                                                            {TEST_LIST_ARG(threadAndMutexTest)},
                                                            {TEST_LIST_ARG(logSystemHeapUsage)}});

    #else  // !USE_GENERATED_EMDEVIF_TEST_LIST

        #include "emdevif_test_list.generated.hpp"

    #endif  // !USE_GENERATED_EMDEVIF_TEST_LIST

#endif      // !EMDEVIF_TEST_LIST_HPP
