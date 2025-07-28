cmake_minimum_required(VERSION 3.28)

project(emdevif_thread C CXX)

add_library(${PROJECT_NAME} OBJECT)

add_subdirectory(system)

target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_core)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET emdevif_thread_exported_modules_unit TYPE CXX_MODULES FILES
        message_queue.cppm
        thread_constant.cppm

    FILE_SET emdevif_thread_modules TYPE CXX_MODULES FILES
        Interface/msg_queue_interface.cppm
        Interface/thread_constant_interface.cppm
)

if (EMDEVIF_THREAD_SYSTEM STREQUAL "FreeRTOS")
    option(EMDEVIF_FREERTOS_USE_STM32CUBEMX "FreeRTOS will use stm32cubemx generated FreeRTOS sources" OFF)

    add_subdirectory(Implements/FreeRTOS)
    target_link_libraries(${PROJECT_NAME} PUBLIC emdevif__freertos_impl)
endif ()
