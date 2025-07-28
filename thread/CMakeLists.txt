cmake_minimum_required(VERSION 3.28)

project(emdevif_thread CXX)

add_library(${PROJECT_NAME} OBJECT)

add_subdirectory(system)

target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_core)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET emdevif_thread_modules TYPE CXX_MODULES FILES
        message_queue.cppm
        thread_constant.cppm
)
