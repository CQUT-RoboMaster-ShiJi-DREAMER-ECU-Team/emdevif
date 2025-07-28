cmake_minimum_required(VERSION 3.28)

project(emdevif__freertos_impl C CXX)

add_library(${PROJECT_NAME} OBJECT)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET emdevif__freertos_impl_modules TYPE CXX_MODULES FILES
        msg_queue_implements.cppm
        thread_constant_implements.cppm
)

target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_core)

if (EMDEVIF_FREERTOS_USE_STM32CUBEMX)
    if (NOT EMDEVIF_USE_STM32CUBEMX)
        message(FATAL_ERROR "Let FreeRTOS to use stm32cubemx requires to set EMDEVIF_USE_STM32CUBEMX equal to ON")
    endif ()

    target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_stm32cubemx_support)
else ()
    target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_freertos_support)
endif ()
