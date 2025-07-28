cmake_minimum_required(VERSION 3.28)

project(emdevif_core C CXX)

add_library(${PROJECT_NAME} OBJECT)

target_compile_options(${PROJECT_NAME} PUBLIC
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti -fmodules-ts -fno-exceptions>
)

target_include_directories(${PROJECT_NAME} PUBLIC inc/)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_core_module TYPE CXX_MODULES FILES
        error_handler/error_handler.cppm

    PUBLIC error_handler/error_handler.cpp
)
