cmake_minimum_required(VERSION 3.28)

project(emdevif_core C CXX)

add_library(${PROJECT_NAME} OBJECT)

if (NOT (DEFINED EMDEVIF_RELEASE_OPTIMIZE_LEVEL))
    set(EMDEVIF_RELEASE_OPTIMIZE_LEVEL -O2)
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(EMDEVIF__COMPILE_OPTIMIZE -O0 -g3)
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    set(EMDEVIF__COMPILE_OPTIMIZE ${EMDEVIF_RELEASE_OPTIMIZE_LEVEL} -g0)
elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebugInfo")
    set(EMDEVIF__COMPILE_OPTIMIZE ${EMDEVIF_RELEASE_OPTIMIZE_LEVEL} -g3)
else ()
    set(EMDEVIF__COMPILE_OPTIMIZE -O0 -g3)
endif ()

target_compile_options(${PROJECT_NAME} PUBLIC
    -Wall -Wextra -Wpedantic
    ${EMDEVIF__COMPILE_OPTIMIZE}
    $<$<COMPILE_LANGUAGE:CXX>: -fmodules-ts>
)

target_include_directories(${PROJECT_NAME} PUBLIC inc/)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_core_module TYPE CXX_MODULES FILES
        error_handler/error_handler.cppm

    PUBLIC error_handler/error_handler.cpp
)
