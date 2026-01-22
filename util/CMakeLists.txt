cmake_minimum_required(VERSION 3.28...4.0.2)

project(emdevif_util CXX)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core
    PRIVATE emdevif_compile_options
)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_util_export_module_units TYPE CXX_MODULES FILES
        InitOnce.cppm
        BitInt.cppm

    PUBLIC FILE_SET emdevif_util_modules TYPE CXX_MODULES FILES
        Implements/BitInt/bit_int_base.cppm
        Implements/BitInt/signed_bit_int.cppm
        Implements/BitInt/unsigned_bit_int.cppm

    PRIVATE FILE_SET emdevif_util_internal_headers TYPE HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/internal_headers/
        FILES internal_headers/BitInt_exception_config.hpp
)
