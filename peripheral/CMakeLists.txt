cmake_minimum_required(VERSION 3.28)

project(emdevif_peripheral CXX)

add_subdirectory(model)
add_subdirectory(peripheral_handle_map)
add_subdirectory(peripheral_impl)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE
    emdevif_peripheral_model emdevif_peripheral_impl_
)
