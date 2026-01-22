cmake_minimum_required(VERSION 3.28...4.0.2)

project(emdevif_peripheral_handle_map_ CXX)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core emdevif_user_declares
    PRIVATE emdevif_compile_options
)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_device_handle_map_modules TYPE CXX_MODULES FILES
        PeripheralHandleMap.cppm
)
