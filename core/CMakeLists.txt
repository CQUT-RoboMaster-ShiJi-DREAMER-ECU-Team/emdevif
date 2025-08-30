cmake_minimum_required(VERSION 3.28)

include(cmake/config_optimize.cmake)

project(emdevif_core C CXX)

add_library(${PROJECT_NAME} OBJECT)

target_include_directories(${PROJECT_NAME}
    PUBLIC inc/
    PRIVATE emdevif_compile_options
)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_core_module TYPE CXX_MODULES FILES
        error_handler/error_handler.cppm

    PUBLIC error_handler/error_handler.cpp
)
