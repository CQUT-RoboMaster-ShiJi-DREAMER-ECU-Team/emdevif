cmake_minimum_required(VERSION 3.28)

project(emdevif_stm32cubemx_support C CXX ASM)

add_library(${PROJECT_NAME} OBJECT)

target_link_libraries(${PROJECT_NAME} PUBLIC stm32cubemx)
