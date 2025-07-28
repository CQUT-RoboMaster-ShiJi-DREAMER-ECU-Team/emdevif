cmake_minimum_required(VERSION 3.28)

project(emdevif_util CXX)

add_library(${PROJECT_NAME} OBJECT)

target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_core)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET emdevif_util_modules TYPE CXX_MODULES FILES
        ArrayTraitsAggregate.cppm
        InitOnce.cppm
)
