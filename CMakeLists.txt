cmake_minimum_required(VERSION 3.28)

project(emdevif C CXX)

set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

option(EMDEVIF_ENABLE_TESTS "Enable the test for emdevif." OFF)
option(EMDEVIF_USE_STM32CUBEMX "Enable it will link stm32cubemx lib to emdevif" OFF)

if (EMDEVIF_USE_STM32CUBEMX)
    enable_language(ASM)
    add_subdirectory(support/stm32cubemx)
endif ()

add_subdirectory(core)
add_subdirectory(logger)
add_subdirectory(timeline)
add_subdirectory(system)
add_subdirectory(peripheral_handle_map)
add_subdirectory(peripherals)
add_subdirectory(util)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE
    emdevif_core
    emdevif_logger
    emdevif_timeline
    emdevif_system
    emdevif_peripheral_handle_map
    emdevif_peripheral
    emdevif_util
)

if (EMDEVIF_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(test)
    message("[${PROJECT_NAME}]: Enabled tests.")
endif ()
