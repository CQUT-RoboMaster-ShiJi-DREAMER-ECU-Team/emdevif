cmake_minimum_required(VERSION 3.28)

project(emdevif_unit_test C CXX)

add_subdirectory(emdevif_test_framework)
add_subdirectory(heap_usage_checker)

set(EMDEVIF_TEST_IGNORE_NAME_LIST "\"\"" CACHE STRING "Set to ignore specific emdevif test. \
You can use \';\' to set multiple tests(eg: set it equals to \"loggerTest;heapTest\" will ignore \
loggerTest and heapTest)."
)

add_library(${PROJECT_NAME} STATIC)

#[==[
# @function testForceToIgnore(<in_out_testIgnoreList> <forcedToIgnoreList>)
#
# 强制忽略给定测试
# @param[in, out] in_out_testIgnoreList 待判断的测试忽略列表，将返回强制忽略后的测试忽略列表
# @param forcedToIgnoreList 需要强制忽略的测试列表。
# 如果 in_out_testIgnoreList 中没有 forcedToIgnoreList 中的元素，将给出提示，并强制补充
# in_out_testIgnoreList 中没有的值。
]==]
function(testForceToIgnore in_out_testIgnoreList forcedToIgnoreList)
    set(testIgnoreList ${in_out_testIgnoreList})

    set(unignoredItemList "")
    foreach (item IN LISTS ${forcedToIgnoreList})
        list(FIND testIgnoreList ${item} testIgnoreListHaveIgnoredThis)

        if (NOT (testIgnoreListHaveIgnoredThis EQUAL -1))
            list(APPEND unignoredItemList ${item})
        endif ()
    endforeach ()

    if (NOT (unignoredItemList STREQUAL ""))
        message(NOTICE "Function `${CMAKE_CURRENT_FUNCTION}\': "
            "Should add this test(s) into ignore list: ${unignoredItemList} ,\n"
            "but have not add in fact."
        )
    endif ()
    list(APPEND testIgnoreList ${unignoredItemList})

    set(${in_out_testIgnoreList} ${testIgnoreList} PARENT_SCOPE)
endfunction()

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
    PRIVATE emdevif emdevif_test_framework emdevif_compile_options
            emdevif_test_heap_checker emdevif_user_declares
)
