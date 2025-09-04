cmake_minimum_required(VERSION 3.28)

project(emdevif_peripheral_handle_map CXX)

if ((NOT (TARGET emdevif_compile_options)) OR
    (NOT (TARGET emdevif_core)) OR
    (NOT (TARGET emdevif_container)))
    message(FATAL_ERROR "${PROJECT_NAME} requires emdevif_compile_options, emdevif_core and emdevif_container!")
endif ()

if (NOT (TARGET emdevif_from_user_impl))
    message(FATAL_ERROR "${PROJECT_NAME} requires user to implements lib \"emdevif_from_user_impl\". \
Please implements it.")
endif ()

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core emdevif_container emdevif_from_user_impl
    PRIVATE emdevif_compile_options
)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_device_handle_map_modules TYPE CXX_MODULES FILES
        PeripheralHandleMap.cppm
)
