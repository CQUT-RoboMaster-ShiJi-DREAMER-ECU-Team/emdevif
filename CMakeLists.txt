cmake_minimum_required(VERSION 3.28)

project(emdevif C CXX)

set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

option(EMDEVIF_USE_STM32CUBEMX "Enable it will link stm32cubemx lib to emdevif" OFF)

if (EMDEVIF_USE_STM32CUBEMX)
    enable_language(ASM)
    add_subdirectory(support/stm32cubemx)
endif ()

add_subdirectory(core)
add_subdirectory(system)
add_subdirectory(connectivity)
add_subdirectory(util)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME} PUBLIC
    emdevif_core
    emdevif_system
    emdevif_connectivity
    emdevif_util
)

if (EMDEVIF_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
