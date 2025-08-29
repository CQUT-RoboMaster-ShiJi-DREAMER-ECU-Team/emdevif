cmake_minimum_required(VERSION 3.28)

project(emdevif_core C CXX)

add_library(${PROJECT_NAME} OBJECT)

set(CMAKE_C_FLAGS_DEBUG "" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE  "" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELWITHDEBINFO  "" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_MINSIZEREL  "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE  "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL  "" CACHE STRING "" FORCE)

set(EMDEVIF_RELEASE_OPTIMIZE_LEVEL "2" CACHE STRING "")

if (MSVC)
    if (EMDEVIF_RELEASE_OPTIMIZE_LEVEL STREQUAL "0")
        set(EMDEVIF_RELEASE_OPTIMIZE_LEVEL "d")
    endif ()

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(EMDEVIF__COMPILE_OPTIMIZE /Od /DEBUG)
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(EMDEVIF__COMPILE_OPTIMIZE "/O${EMDEVIF_RELEASE_OPTIMIZE_LEVEL}")
    elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebugInfo")
        set(EMDEVIF__COMPILE_OPTIMIZE "/O${EMDEVIF_RELEASE_OPTIMIZE_LEVEL};/DEBUG")
    else ()
        message(WARNING "Unknown build type, use /Od /DEBUG same as Debug.")
        set(EMDEVIF__COMPILE_OPTIMIZE /Od /DEBUG)
    endif ()
else ()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(EMDEVIF__COMPILE_OPTIMIZE -O0 -g3)
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(EMDEVIF__COMPILE_OPTIMIZE "-O${EMDEVIF_RELEASE_OPTIMIZE_LEVEL};-g0;-DNDEBUG")
    elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(EMDEVIF__COMPILE_OPTIMIZE "-O${EMDEVIF_RELEASE_OPTIMIZE_LEVEL};-g3;-DNDEBUG")
    else ()
        message(WARNING "Unknown build type, use -O0 -g3 same as Debug.")
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
    )
endif ()

target_include_directories(${PROJECT_NAME} PUBLIC inc/)

target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET emdevif_core_module TYPE CXX_MODULES FILES
        error_handler/error_handler.cppm

    PUBLIC error_handler/error_handler.cpp
)
