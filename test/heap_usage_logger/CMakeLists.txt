cmake_minimum_required(VERSION 3.28)

project(emdevif_test_heap_logger C)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC FILE_SET header TYPE HEADERS
    FILES heap_usage_logger.hpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    freertos_kernel emdevif_test_framework emdevif_compile_options
    nva_print
)

if ((DEFINED EMDEVIF_THREAD_SYSTEM) AND (EMDEVIF_THREAD_SYSTEM STREQUAL "FreeRTOS"))
    target_sources(${PROJECT_NAME} PRIVATE FreeRTOS/FreeRTOS_heap_logger.cpp)
endif ()
