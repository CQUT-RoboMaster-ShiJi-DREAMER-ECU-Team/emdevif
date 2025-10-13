cmake_minimum_required(VERSION 3.28)

project(emdevif_peripherals CXX)

add_subdirectory(models)
add_subdirectory(peripherals)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE
    emdevif_peripheral_models emdevif_peripheral_behaviours
)
