cmake_minimum_required(VERSION 3.28)

project(rmdev_thread_system_support CXX)

add_library(${PROJECT_NAME} OBJECT)

if (RMDEV_USE_STM32CUBEMX)
    enable_language(ASM)
    target_link_libraries(${PROJECT_NAME} PRIVATE stm32cubemx)
endif ()

file(GLOB_RECURSE SYSTEM_FILES "Implements/*.cppm" "Interface/*.cppm")
target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET rmdev_system_modules TYPE CXX_MODULES FILES
            ${SYSTEM_FILES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_core)
