cmake_minimum_required(VERSION 3.28)

include(cmake/config_compile_options.cmake)

project(emdevif_core C CXX)

add_library(${PROJECT_NAME} STATIC)

set(BOOST_USE_MODULES ON CACHE BOOL "" FORCE)
add_subdirectory(depends/boost_pfr)

target_link_libraries(${PROJECT_NAME} PUBLIC boost_pfr)

target_include_directories(${PROJECT_NAME}
    PUBLIC inc/
    PRIVATE emdevif_compile_options
)

file(GLOB_RECURSE emdevifCoreModules
    "error_handler/*.cppm"
    "data_container/*.cppm"
    "type_traits/*.cppm"
)
target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET ${PROJECT_NAME}_modules TYPE CXX_MODULES FILES
        ${emdevifCoreModules}
        endian.cppm
        concepts.cppm

    PRIVATE
        error_handler/error_handler.cpp
        error_handler/fatal_handler_extern_c.cpp
)
