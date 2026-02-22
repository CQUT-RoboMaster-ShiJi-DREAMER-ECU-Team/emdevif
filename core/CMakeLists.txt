cmake_minimum_required(VERSION 3.28...4.0.2)

include(cmake/config_compile_options.cmake)

project(emdevif_core C CXX)

option(EMDEVIF_CORE_USE_BUILT_IN_BOOST_PFR_LIB
    "Enabled it to use built-in Boost.PFR library. If disabled it, you should \
provide `Boost::pfr\' alias library(Use find_package, FetchContent or built-in \
Boost.PFR library on another place)."
    ON
)

add_library(${PROJECT_NAME} STATIC)

if (EMDEVIF_USE_CPP_MODULES)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EMDEVIF_USE_MODULES=1)
    set(BOOST_USE_MODULES ON CACHE BOOL
        "This value was set to ON with \"FORCE\"(Because of `EMDEVIF_USE_CPP_MODULES\' \
is ON), DO NOT EDIT IT!"
        FORCE
    )
else ()
    set(BOOST_USE_MODULES OFF CACHE BOOL
        "This value was set to OFF with \"FORCE\"(Because of `EMDEVIF_USE_CPP_MODULES\' \
is OFF), DO NOT EDIT IT!"
        FORCE
    )
endif ()

if (EMDEVIF_CORE_USE_BUILT_IN_BOOST_PFR_LIB)
    add_subdirectory(depends/boost_pfr)
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::pfr)

target_include_directories(${PROJECT_NAME} PUBLIC inc/)

file(GLOB_RECURSE emdevifCoreModules "modules/*.cppm" "data_container/*.cppm")
target_sources(${PROJECT_NAME}
    PRIVATE
        src/error_handler/error_handler.cpp
        src/error_handler/fatal_handler_extern_c.cpp

    PUBLIC FILE_SET ${PROJECT_NAME}_modules TYPE CXX_MODULES FILES
        ${emdevifCoreModules}
)
