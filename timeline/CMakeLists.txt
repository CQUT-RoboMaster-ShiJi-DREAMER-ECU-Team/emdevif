cmake_minimum_required(VERSION 3.28...4.0.2)

project(emdevif_timeline CXX)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core
    PRIVATE emdevif_compile_options emdevif_user_declares
)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET ${PROJECT_NAME}_modules TYPE CXX_MODULES FILES
        Timeline.cppm
)
