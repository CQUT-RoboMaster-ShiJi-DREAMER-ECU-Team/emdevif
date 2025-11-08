#[==[
# @file utility.cmake
# @brief CMake 实用功能
]==]

#[==[
# @function emdevif_trueOptionsCount(<out_result> ...)
#
# 用于获取给定的布尔类型变量中，真值的数量
# @param[out] out_result 返回值为真的变量数
# @param ... 待计数的变量。可以有多个，用空格分隔。
]==]
function(emdevif_trueOptionsCount out_result)
    set(trueCount 0)

    foreach (item IN LISTS ARGN)
        if (${item})
            math(EXPR trueCount "${trueCount} + 1")
        endif ()
    endforeach ()

    set(${out_result} ${trueCount} PARENT_SCOPE)
endfunction()

#[==[
# @function emdevif_assertMutuallyExclusiveOptions(...)
#
# 用于断言给定的布尔类型变量中，真值的数量最多 1 个
# @param ... 待断言的变量。可以有多个，用空格分隔。
]==]
function(emdevif_assertMutuallyExclusiveOptions)
    emdevif_trueOptionsCount(result ${ARGN})

    if (result GREATER_EQUAL 2)
        message(FATAL_ERROR "Assert `mutuallyExclusiveOptions\' Failed.")
    endif ()
endfunction()
