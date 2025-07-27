cmake_minimum_required(VERSION 3.28)

project(rmdev_connectivity CXX)

add_library(${PROJECT_NAME} OBJECT)

target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_core rmdev_util)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET rmdev_connectivity_modules TYPE CXX_MODULES FILES
        Connect.cppm
        Serial.cppm
)
