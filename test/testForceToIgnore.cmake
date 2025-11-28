include_guard()

#[==[
# @function testForceToIgnore(<in_testIgnoreList> <out_testIgnoreList> <forcedToIgnoreList>)
#
# 强制忽略给定测试
# @param in_testIgnoreList 待判断的测试忽略列表
# @param[out] out_testIgnoreList 返回强制忽略后的测试忽略列表
# @param forcedToIgnoreList 需要强制忽略的测试列表。
# 如果 in_testIgnoreList 中没有 forcedToIgnoreList 中的元素，将给出提示，并强制补充
# in_testIgnoreList 中没有的值到 out_testIgnoreList。
]==]
function(testForceToIgnore in_testIgnoreList out_testIgnoreList forcedToIgnoreList)
    set(testIgnoreList ${in_testIgnoreList})
    set(forcedToIgnoreList_ ${forcedToIgnoreList})

    message(DEBUG "[${PROJECT_NAME}]: In function `${CMAKE_CURRENT_FUNCTION}\': "
        "Pre: "
        "testIgnoreList=\"${testIgnoreList}\", forcedToIgnoreList=\"${forcedToIgnoreList}\", "
        "forcedToIgnoreList_=\"${forcedToIgnoreList_}\""
    )
    list(APPEND CMAKE_MESSAGE_INDENT " >> ")

    set(unignoredItemList "")
    foreach (item IN LISTS forcedToIgnoreList_)
        list(FIND testIgnoreList ${item} testIgnoreListHaveIgnoredThis)

        if (testIgnoreListHaveIgnoredThis EQUAL -1)
            list(APPEND unignoredItemList ${item})

            message(DEBUG
                "item=\"${item}\", testIgnoreListHaveIgnoredThis=\"${testIgnoreListHaveIgnoredThis}\". "
                "This item appended in unignoredItemList(=\"${unignoredItemList}\"), becauuse we need to "
                "add it into ignore list, but have not in fact."
            )
        else ()
            message(DEBUG
                "item=\"${item}\", testIgnoreListHaveIgnoredThis=\"${testIgnoreListHaveIgnoredThis}\". "
                "This item have in testIgnoreList(=\"${testIgnoreList}\") already, so did nothing."
            )
        endif ()
    endforeach ()

    if (NOT (unignoredItemList STREQUAL ""))
        message(NOTICE "[${PROJECT_NAME}]: In function `${CMAKE_CURRENT_FUNCTION}\': "
            "Should add this test(s) into ignore list: \"${unignoredItemList}\", "
            "but have not add them in fact. So this script supplemented them."
        )
    endif ()
    list(APPEND testIgnoreList ${unignoredItemList})

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(DEBUG "[${PROJECT_NAME}]: In function `${CMAKE_CURRENT_FUNCTION}\': "
        "Post: "
        "testIgnoreList=\"${testIgnoreList}\", forcedToIgnoreList=\"${forcedToIgnoreList}\", "
        "unignoredItemList=\"${unignoredItemList}\""
    )

    set(${out_testIgnoreList} ${testIgnoreList} PARENT_SCOPE)
endfunction()
