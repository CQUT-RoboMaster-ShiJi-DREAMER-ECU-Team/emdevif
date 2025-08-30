cmake_minimum_required(VERSION 3.28)

project(emdevif_unit_test C CXX)

add_subdirectory(emdevif_test_framework)

add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE EMDEVIF_TEST_SUITS_SRC "test_suits/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE
    test_main.cpp

    ${EMDEVIF_TEST_SUITS_SRC}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE emdevif emdevif_test_framework emdevif_compile_options
)
