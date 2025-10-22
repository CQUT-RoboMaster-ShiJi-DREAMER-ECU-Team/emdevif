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

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET ${PROJECT_NAME}_error_handler TYPE CXX_MODULES FILES
        error_handler/error_handler.cppm
        error_handler/error_code.cppm
        error_handler/fatal_handler.cppm
        error_handler/ErrorMessage.cppm

        endian.cppm

    PRIVATE error_handler/error_handler.cpp error_handler/fatal_handler_extern_c.cpp

    PUBLIC FILE_SET ${PROJECT_NAME}_type_traits TYPE CXX_MODULES FILES
        type_traits/type_traits.cppm
        type_traits/impl/misc.cppm
        type_traits/impl/tuple_style.cppm

    PUBLIC FILE_SET ${PROJECT_NAME}_container TYPE CXX_MODULES FILES
        data_container/Map.cppm
        data_container/implements/Map/ArrayMap.cppm
)
