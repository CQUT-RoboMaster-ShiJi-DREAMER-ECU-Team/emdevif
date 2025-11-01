
function(emdevif_trueOptionsCount out_result)
    set(trueCount 0)

    foreach (item IN ${ARGN})
        if (item)
            math(EXPR trueCount "${trueCount} + 1")
        endif ()
    endforeach ()

    set(${out_result} ${trueCount} PARENT_SCOPE)
endfunction()

function(emdevif_assertMutuallyExclusiveOptions)
    emdevif_trueOptionsCount(result ${ARGN})

    if (result GREATER_EQUAL 2)
        message(FATAL_ERROR "Assert `mutuallyExclusiveOptions\' Failed.")
    endif ()
endfunction()
