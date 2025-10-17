cmake_minimum_required(VERSION 3.28)

project(emdevif_unit_test C CXX)

add_subdirectory(emdevif_test_framework)
add_subdirectory(heap_usage_checker)

add_library(${PROJECT_NAME} STATIC)

block()
    file(GLOB_RECURSE emdevifTestSuitsSrc "test_suits/*.cpp" "test_suits/*.c")
    target_sources(${PROJECT_NAME} PRIVATE
        test_main.cpp

        ${emdevifTestSuitsSrc}
    )
endblock()

target_link_libraries(${PROJECT_NAME}
    PRIVATE emdevif emdevif_test_framework emdevif_compile_options
            emdevif_test_heap_checker emdevif_user_declares
)
