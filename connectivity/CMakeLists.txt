cmake_minimum_required(VERSION 3.28)

project(emdevif_connectivity CXX)

add_library(${PROJECT_NAME} OBJECT)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core emdevif_util emdevif_peripheral_handle_map
    PRIVATE emdevif_compile_options
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET emdevif_connectivity_modules TYPE CXX_MODULES FILES
        Serial.cppm
        Can.cppm
        Spi.cppm
)
