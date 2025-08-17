cmake_minimum_required(VERSION 3.28)

project(emdevif_util CXX)

add_library(${PROJECT_NAME} OBJECT)

target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_core)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_util_export_module_units TYPE CXX_MODULES FILES
        InitOnce.cppm
        BitInt.cppm

    PUBLIC FILE_SET emdevif_module TYPE CXX_MODULES FILES
        Implements/BitInt/signed_bit_int.cppm
        Implements/BitInt/unsigned_bit_int.cppm
)
