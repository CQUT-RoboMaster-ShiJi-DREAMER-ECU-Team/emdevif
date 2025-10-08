cmake_minimum_required(VERSION 3.28)

project(emdevif_peripheral_model_traits CXX)

add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET ${PROJECT_NAME}_modules TYPE CXX_MODULES FILES gpio_traits.cppm
)
target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_core PRIVATE emdevif_compile_options)
