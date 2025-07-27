cmake_minimum_required(VERSION 3.28)

project(rmdev_util CXX)

add_library(${PROJECT_NAME} OBJECT)

target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_core)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET rmdev_util_modules TYPE CXX_MODULES FILES
        ArrayTraitsAggregate.cppm
        InitOnce.cppm
)
