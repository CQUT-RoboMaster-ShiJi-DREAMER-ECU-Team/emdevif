cmake_minimum_required(VERSION 3.28...4.0.2)

project(emdevif C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_subdirectory(check_compile_features)

set(emdevifAllModules "logger;timeline;system;peripheral;util")

option(EMDEVIF_ENABLE_TESTS "Enable the test for emdevif." OFF)
set(EMDEVIF_ENABLED_MODULES ${emdevifAllModules} CACHE STRING
    "Set the modules to be enabled in emdevif, separated by semicolons(\'\;\'). \
Default value is all modules(Listed in local variable `emdevifAllModules\')."
)

option(EMDEVIF_USE_STM32CUBEMX "Enable it will link stm32cubemx lib to emdevif" OFF)
option(EMDEVIF_USE_ESP_IDF "Enable it to use ESP-IDF." OFF)

include(cmake/utility.cmake)

emdevif_assertMutuallyExclusiveOptions(EMDEVIF_USE_STM32CUBEMX EMDEVIF_USE_ESP_IDF)

if (EMDEVIF_USE_STM32CUBEMX)
    enable_language(ASM)
    add_subdirectory(support/stm32cubemx)
elseif (EMDEVIF_USE_ESP_IDF)
    add_subdirectory(support/esp-idf)
endif ()

add_subdirectory(core)

add_library(${PROJECT_NAME} INTERFACE)

if (EMDEVIF_ENABLE_TESTS)
    message(STATUS "[${PROJECT_NAME}]: Enabled tests, will link all modules.")

    set(emdevifFinallyEnabledModules ${emdevifAllModules})
else ()
    set(emdevifFinallyEnabledModules ${EMDEVIF_ENABLED_MODULES})
endif ()

set(emdevifFinallyLinkedLibs "")
foreach (moduleName IN LISTS emdevifFinallyEnabledModules)
    add_subdirectory(${moduleName})

    list(APPEND emdevifFinallyLinkedLibs "emdevif_${moduleName}")

    message(STATUS "[${PROJECT_NAME}]: Enabled module: emdevif_${moduleName}")
endforeach ()

target_link_libraries(${PROJECT_NAME} INTERFACE
    emdevif_core
    ${emdevifFinallyLinkedLibs}
)

if (EMDEVIF_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
