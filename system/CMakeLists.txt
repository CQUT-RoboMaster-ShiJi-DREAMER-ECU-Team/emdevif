cmake_minimum_required(VERSION 3.28)

project(emdevif_system C CXX)

set(EMDEVIF_THREAD_SYSTEM "FreeRTOS" CACHE STRING "Select witch system to use. \
Default value is \"FreeRTOS\"")

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core
    PRIVATE emdevif_compile_options emdevif_util
)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_thread_exported_modules_unit TYPE CXX_MODULES FILES
        sys_queue.cppm
        thread.cppm
        atomic.cppm
        mutex.cppm
        semaphore.cppm
        event_group.cppm
        heap_memory.cppm

        sys_message_slot.cppm

    PUBLIC FILE_SET emdevif_thread_interface_modules TYPE CXX_MODULES FILES
        Interface/sys_queue_interface.cppm
        Interface/thread_interface.cppm
        Interface/mutex_interface.cppm
        Interface/semaphore_interface.cppm
        Interface/event_group_interface.cppm
)

if (EMDEVIF_USE_ESP_IDF)
    if (NOT (EMDEVIF_THREAD_SYSTEM STREQUAL "FreeRTOS"))
        message(WARNING "[${PROJECT_NAME}]: Usage ESP-IDF, so the value EMDEVIF_THREAD_SYSTEM\
        (was set to `${EMDEVIF_THREAD_SYSTEM}\') ignored and forced to be set with `FreeRTOS\'.")

        set(EMDEVIF_THREAD_SYSTEM "FreeRTOS")
    endif ()
endif ()

if (EMDEVIF_THREAD_SYSTEM STREQUAL "FreeRTOS")
    option(EMDEVIF_FREERTOS_USE_STM32CUBEMX "FreeRTOS will use stm32cubemx generated FreeRTOS sources" OFF)

    if (EMDEVIF_USE_ESP_IDF)
        if (EMDEVIF_FREERTOS_USE_STM32CUBEMX)
            message(WARNING "[${PROJECT_NAME}]: EMDEVIF_FREERTOS_USE_STM32CUBEMX is set to ON while usage \
            ESP-IDF(EMDEVIF_USE_ESP_IDF is ON), ignored.")
        endif ()

        target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_esp_idf_support)
    elseif (EMDEVIF_FREERTOS_USE_STM32CUBEMX)
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
            Implements/FreeRTOS/sys_queue_implements.cppm
            Implements/FreeRTOS/thread_implements.cppm
            Implements/FreeRTOS/mutex_implements.cppm
            Implements/FreeRTOS/semaphore_implements.cppm
            Implements/FreeRTOS/event_group_implements.cppm
            Implements/FreeRTOS/event_group_definitions.cppm
            Implements/FreeRTOS/heap_memory_implements.cppm
    )
else ()
    if (NOT (TARGET emdevif_system_impl))
        message(FATAL_ERROR "If EMDEVIF_THREAD_SYSTEM not equal to FreeRTOS, it requires \
target `emdevif_system_impl\' to implements system interface.")
    endif ()

    target_link_libraries(${PROJECT_NAME} PUBLIC emdevif_system_impl)
endif ()

set(additionalMessage "")
if ((EMDEVIF_THREAD_SYSTEM STREQUAL "FreeRTOS") AND (EMDEVIF_FREERTOS_USE_STM32CUBEMX))
    set(additionalMessage " Use STM32CubeMX generated FreeRTOS lib.")
endif ()
message(STATUS "[${PROJECT_NAME}]: Use \"${EMDEVIF_THREAD_SYSTEM}\".${additionalMessage}")
