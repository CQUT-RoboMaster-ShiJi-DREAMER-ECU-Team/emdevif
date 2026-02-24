cmake_minimum_required(VERSION 3.28...4.0.2)

project(emdevif_logger C CXX)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core
    PRIVATE emdevif_compile_options emdevif_user_declares emdevif_system
)

target_include_directories(${PROJECT_NAME} PUBLIC inc)

target_sources(${PROJECT_NAME}
    PRIVATE src/logger.cpp

    PUBLIC
    FILE_SET emdevif_logger_modules TYPE CXX_MODULES FILES
        Logger.cppm
)
