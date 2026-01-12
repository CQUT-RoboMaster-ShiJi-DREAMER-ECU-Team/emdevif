cmake_minimum_required(VERSION 3.28)

include(cmake/config_compile_options.cmake)

project(emdevif_core C CXX)

option(EMDEVIF_CORE_USE_BUILT_IN_BOOST_PFR_LIB
    "Enabled it to use built-in Boost.PFR library. If disabled it, you should \
provide `Boost::pfr\' alias library(Use find_package, FetchContent or built-in \
Boost.PFR library on another place)."
    ON
)

add_library(${PROJECT_NAME} STATIC)

set(BOOST_USE_MODULES ON CACHE BOOL "" FORCE)
if (EMDEVIF_CORE_USE_BUILT_IN_BOOST_PFR_LIB)
    add_subdirectory(depends/boost_pfr)
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::pfr)

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
        endian.cppm concepts.cppm lock_guard.cppm

    PRIVATE
        error_handler/error_handler.cpp
        error_handler/fatal_handler_extern_c.cpp
)
