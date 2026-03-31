cmake_minimum_required(VERSION 3.28...4.0.2)

project(emdevif_logger C CXX)

set(EMDEVIF_LOGGER_LINK_SYSTEM_LIB ON CACHE BOOL
    "Enabled to link emdevif_system library to emdevif_logger, which is required by some logger modes"
)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core
    PRIVATE emdevif_compile_options emdevif_user_declares
)

if (EMDEVIF_LOGGER_LINK_SYSTEM_LIB)
    target_link_libraries(${PROJECT_NAME} PRIVATE emdevif_system)
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC inc)

target_sources(${PROJECT_NAME} PRIVATE src/logger.cpp)

if (EMDEVIF_USE_CPP_MODULES)
    target_sources(${PROJECT_NAME} PUBLIC FILE_SET emdevif_logger_modules TYPE CXX_MODULES
        FILES logger.cppm
    )
endif ()
