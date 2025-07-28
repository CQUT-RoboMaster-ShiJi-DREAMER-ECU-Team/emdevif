cmake_minimum_required(VERSION 3.28)

project(emdevif C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_subdirectory(core)
add_subdirectory(thread)
add_subdirectory(connectivity)
add_subdirectory(util)

add_library(${PROJECT_NAME} OBJECT)

target_link_libraries(${PROJECT_NAME} PUBLIC
    emdevif_core
    emdevif_thread
    emdevif_connectivity
    emdevif_util
)
