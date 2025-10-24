cmake_minimum_required(VERSION 3.28)

project(emdevif C CXX)

set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(emdevifAllModules "logger;timeline;system;peripheral_handle_map;peripheral;util")

option(EMDEVIF_ENABLE_TESTS "Enable the test for emdevif." OFF)
option(EMDEVIF_USE_STM32CUBEMX "Enable it will link stm32cubemx lib to emdevif" OFF)
set(EMDEVIF_ENABLED_MODULES ${emdevifAllModules} CACHE STRING
    "Set the modules to be enabled in emdevif, separated by semicolons(\'\;\'). \
Default value is all modules(Listed in local variable `emdevifAllModules\')."
)

if (EMDEVIF_USE_STM32CUBEMX)
    enable_language(ASM)
    add_subdirectory(support/stm32cubemx)
endif ()

add_subdirectory(core)

add_library(${PROJECT_NAME} INTERFACE)

block()
    if (EMDEVIF_ENABLE_TESTS)
        enable_testing()
        add_subdirectory(test)
        message("[${PROJECT_NAME}]: Enabled tests, will link all modules.")

        set(emdevifFinallyEnabledModules ${emdevifAllModules})
    else ()
        set(emdevifFinallyEnabledModules ${EMDEVIF_ENABLED_MODULES})
    endif ()

    set(emdevifFinallyLinkedLibs "")
    foreach (moduleName IN LISTS emdevifFinallyEnabledModules)
        add_subdirectory(${moduleName})

        list(APPEND emdevifFinallyLinkedLibs "emdevif_${moduleName}")

        message("[${PROJECT_NAME}]: Enabled module: emdevif_${moduleName}")
    endforeach ()

    target_link_libraries(${PROJECT_NAME} INTERFACE
        emdevif_core
        ${emdevifFinallyLinkedLibs}
    )
endblock()
