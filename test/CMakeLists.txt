cmake_minimum_required(VERSION 3.28...4.0.2)

project(emdevif_unit_test C CXX)

add_subdirectory(emdevif_test_framework)
add_subdirectory(heap_usage_checker)

set(EMDEVIF_TEST_IGNORE_NAME_LIST "" CACHE STRING "Set to ignore specific emdevif test. \
You can use \';\' to set multiple tests(eg: set it equals to \"loggerTest;heapTest\" will ignore \
loggerTest and heapTest)."
)

add_library(${PROJECT_NAME} STATIC)

emdevif_listToCCppStringTypeInitializerList(emdevifUnitTestIgnoreNameList "${EMDEVIF_TEST_IGNORE_NAME_LIST}")
message(STATUS "[${PROJECT_NAME}]: macro `EMDEVIF_TEST_IGNORE_NAME_LIST\' defined to ${emdevifUnitTestIgnoreNameList}")
target_compile_definitions(${PROJECT_NAME} PRIVATE
    EMDEVIF_TEST_IGNORE_NAME_LIST=${emdevifUnitTestIgnoreNameList}
)

file(GLOB_RECURSE emdevifTestSuitsSrc "test_suits/*.cpp" "test_suits/*.c")
target_sources(${PROJECT_NAME} PRIVATE
    test_main.cpp

    ${emdevifTestSuitsSrc}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core
    PRIVATE emdevif emdevif_test_framework emdevif_compile_options
            emdevif_test_heap_checker emdevif_user_declares
)
