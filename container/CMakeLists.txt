cmake_minimum_required(VERSION 3.28)

project(emdevif_container CXX)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME} PRIVATE emdevif_core emdevif_compile_options)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_container_module_units TYPE CXX_MODULES FILES
        Map.cppm

    PUBLIC FILE_SET emdevif_container_module_impl TYPE CXX_MODULES FILES
        implements/Map/StaticMap.cppm
)
