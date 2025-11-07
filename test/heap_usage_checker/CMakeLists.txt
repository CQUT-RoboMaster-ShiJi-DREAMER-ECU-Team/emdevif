cmake_minimum_required(VERSION 3.28)

project(emdevif_test_heap_checker C)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC FILE_SET header TYPE HEADERS
    FILES heap_usage_checker.hpp
)

if (EMDEVIF_FREERTOS_USE_STM32CUBEMX)
    target_link_libraries(${PROJECT_NAME} PRIVATE emdevif_stm32cubemx_support)
elseif (EMDEVIF_USE_ESP_IDF)
    target_link_libraries(${PROJECT_NAME} PRIVATE emdevif_esp_idf_support)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE freertos_kernel)
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE
    emdevif_test_framework emdevif_compile_options
    PUBLIC printf
)

if (EMDEVIF_USE_ESP_IDF)
    target_sources(${PROJECT_NAME} PRIVATE ESP-IDF/esp_idf_heap_checker.cpp)
    message("[${PROJECT_NAME}]: Use ESP-IDF")
elseif (EMDEVIF_THREAD_SYSTEM STREQUAL "FreeRTOS")
    target_sources(${PROJECT_NAME} PRIVATE FreeRTOS/FreeRTOS_heap_checker.cpp)
    message("[${PROJECT_NAME}]: Use FreeRTOS")
endif ()
