cmake_minimum_required(VERSION 3.28)

project(emdevif_device_handle_map CXX)

if ((NOT (TARGET emdevif_compile_options)) OR (NOT (TARGET emdevif_core)))
    message(FATAL_ERROR "${PROJECT_NAME} requires emdevif_compile_options and emdevif_core!")
endif ()

if (NOT (TARGET emdevif_device_handle_map_impl))
    message(FATAL_ERROR "${PROJECT_NAME} requires user to implements lib \"emdevif_device_handle_map_impl\". \
Please implements it.")
endif ()

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME} PRIVATE
    emdevif_core emdevif_compile_options emdevif_device_handle_map_impl
)


