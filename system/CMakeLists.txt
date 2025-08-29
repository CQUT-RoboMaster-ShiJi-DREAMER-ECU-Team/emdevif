cmake_minimum_required(VERSION 3.28)

project(emdevif_system C CXX)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_core)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_thread_exported_modules_unit TYPE CXX_MODULES FILES
        message_queue.cppm
        thread.cppm
        atomic.cppm
        mutex.cppm
        semaphore.cppm
        event_group.cppm

    PUBLIC FILE_SET emdevif_thread_interface_modules TYPE CXX_MODULES FILES
        Interface/msg_queue_interface.cppm
        Interface/thread_interface.cppm
        Interface/mutex_interface.cppm
        Interface/semaphore_interface.cppm
        Interface/event_group_interface.cppm
)

if (EMDEVIF_THREAD_SYSTEM STREQUAL "FreeRTOS")
    option(EMDEVIF_FREERTOS_USE_STM32CUBEMX "FreeRTOS will use stm32cubemx generated FreeRTOS sources" OFF)

    if (EMDEVIF_FREERTOS_USE_STM32CUBEMX)
        if (NOT EMDEVIF_USE_STM32CUBEMX)
            message(FATAL_ERROR "Let FreeRTOS to use stm32cubemx requires to set EMDEVIF_USE_STM32CUBEMX equal to ON")
        endif ()

        target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_stm32cubemx_support)
    else ()
        target_link_libraries(${PROJECT_NAME} PUBLIC freertos_kernel)
    endif ()

    target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_util)

    target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET emdevif__freertos_impl_modules TYPE CXX_MODULES FILES
            Implements/FreeRTOS/msg_queue_implements.cppm
            Implements/FreeRTOS/thread_implements.cppm
            Implements/FreeRTOS/mutex_implements.cppm
            Implements/FreeRTOS/semaphore_implements.cppm
            Implements/FreeRTOS/event_group_implements.cppm
    )
endif ()
