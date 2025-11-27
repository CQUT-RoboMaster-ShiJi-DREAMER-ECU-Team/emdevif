cmake_minimum_required(VERSION 3.28)

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

macro(unitTestRegisterSrc resultSrcList ignoreList testName srcList)
    message(DEBUG "[${PROJECT_NAME}]: In macro `unitTestRegisterSrc\': ignoreList: ${ignoreList}, testName: ${testName}")

    set(ignoreList_ ${ignoreList})
    list(FIND ignoreList_ "${testName}" testNameInIgnoreList)
    if (testNameInIgnoreList EQUAL "-1")
        message(DEBUG "    this test(${testName}) was not in ignore list(${ignoreList_})"
            "(testNameInIgnoreList=${testNameInIgnoreList}), add it in."
        )

        list(APPEND ${resultSrcList} ${srcList})
    else ()
        message(DEBUG "    this test(${testName}) was in ignore list(${ignoreList_})"
            "(testNameInIgnoreList=${testNameInIgnoreList}), ignored."
        )
    endif ()
endmacro()

set(emdevifTestSuitsSrc "")

macro(emdevifUnitTestRegisterSrc testName srcList)
    unitTestRegisterSrc(emdevifTestSuitsSrc "${EMDEVIF_TEST_IGNORE_NAME_LIST}" "${testName}" "${srcList}")
endmacro()

emdevifUnitTestRegisterSrc("cFeatureTest"       "test_suits/language_feature_test/c_feature_test.c")
emdevifUnitTestRegisterSrc("endianTest"         "test_suits/endian_test.cpp")
emdevifUnitTestRegisterSrc("typeTraitsTest"     "test_suits/type_traits_test.cpp")
emdevifUnitTestRegisterSrc("initOnceTest"       "test_suits/util/InitOnce_test.cpp")
emdevifUnitTestRegisterSrc("bitIntTest"         "test_suits/util/BitInt_test.cpp")
emdevifUnitTestRegisterSrc("ringBufferTest"     "test_suits/container/RingBuffer_test.cpp")
emdevifUnitTestRegisterSrc("arrayMapTest"       "test_suits/container/ArrayMap_test.cpp")
emdevifUnitTestRegisterSrc("loggerTest"         "test_suits/logger/Logger_test.cpp")
emdevifUnitTestRegisterSrc("heapTest"           "test_suits/system/heap_test.cpp")
emdevifUnitTestRegisterSrc("threadAndMutexTest" "test_suits/system/thread_mutex_test.cpp")

message(STATUS "[${PROJECT_NAME}]: emdevifTestSuitsSrc = \"${emdevifTestSuitsSrc}\"")
target_sources(${PROJECT_NAME} PRIVATE test_main.cpp ${emdevifTestSuitsSrc})

target_link_libraries(${PROJECT_NAME}
    PRIVATE emdevif emdevif_test_framework emdevif_compile_options
            emdevif_test_heap_checker emdevif_user_declares
)
