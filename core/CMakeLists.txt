cmake_minimum_required(VERSION 3.28)

project(emdevif_core C CXX)

add_library(${PROJECT_NAME} OBJECT)

if (NOT (DEFINED EMDEVIF_RELEASE_OPTIMIZE_LEVEL))
    if (MSVC)
        set(EMDEVIF_RELEASE_OPTIMIZE_LEVEL /O2)
    else ()
        set(EMDEVIF_RELEASE_OPTIMIZE_LEVEL -O2)
    endif ()
endif ()

if (MSVC)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(EMDEVIF__COMPILE_OPTIMIZE /Od /DEBUG)
    elseif (CMAKE_BUILD_TYPE MATCHES "Release")
        set(EMDEVIF__COMPILE_OPTIMIZE ${EMDEVIF_RELEASE_OPTIMIZE_LEVEL})
    elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebugInfo")
        set(EMDEVIF__COMPILE_OPTIMIZE ${EMDEVIF_RELEASE_OPTIMIZE_LEVEL} /DEBUG)
    else ()
        set(EMDEVIF__COMPILE_OPTIMIZE /Od /DEBUG)
    endif ()
else ()
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(EMDEVIF__COMPILE_OPTIMIZE -O0 -g3)
    elseif (CMAKE_BUILD_TYPE MATCHES "Release")
        set(EMDEVIF__COMPILE_OPTIMIZE ${EMDEVIF_RELEASE_OPTIMIZE_LEVEL} -g0)
    elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebugInfo")
        set(EMDEVIF__COMPILE_OPTIMIZE ${EMDEVIF_RELEASE_OPTIMIZE_LEVEL} -g3)
    else ()
        set(EMDEVIF__COMPILE_OPTIMIZE -O0 -g3)
    endif ()
endif ()

option(EMDEVIF_NOT_SET_OPTIMIZE
    "Enabled this to unset EMDEVIF__COMPILE_OPTIMIZE in order to add compile \
option repeatedly (for example, if you have add_compile_option(-O2) already, \
it's best to set EMDEVIF_NOT_SET_OPTIMIZE on to avoid add compile option repeatedly)."
    OFF
)

if (EMDEVIF_NOT_SET_OPTIMIZE)
    unset(EMDEVIF__COMPILE_OPTIMIZE)
endif ()

if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC
        /W4 /source-charset:utf-8 /execution-charset:gb2312
        ${EMDEVIF__COMPILE_OPTIMIZE}
    )
else ()
    target_compile_options(${PROJECT_NAME} PUBLIC
        -Wall -Wextra -Wpedantic
        ${EMDEVIF__COMPILE_OPTIMIZE}
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:GNU>>: -fmodules-ts>
    )
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC inc/)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_core_module TYPE CXX_MODULES FILES
        error_handler/error_handler.cppm

    PUBLIC error_handler/error_handler.cpp
)
