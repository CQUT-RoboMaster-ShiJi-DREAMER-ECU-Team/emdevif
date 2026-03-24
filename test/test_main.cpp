/**
 * @file test_main.cpp
 * @brief 测试主函数
 */

#include <array>
#include <utility>
#include <string_view>
#include <algorithm>

#include "test_main.hpp"
#include "emdevif_test_list.hpp"

#ifndef EMDEVIF_TEST_IGNORE_NAME_LIST
    #error \
        "Please define the macro `EMDEVIF_TEST_IGNORE_NAME_LIST'. If nothing to ignore, define it to {""} ."
#endif

static constexpr auto emdevif_test_ignore_name_list = std::to_array<std::string_view>(EMDEVIF_TEST_IGNORE_NAME_LIST);

static constexpr bool inIgnoreNameList(const std::string_view& name) noexcept
{
    return std::ranges::any_of(emdevif_test_ignore_name_list, [&name](const auto& it) { return it == name; });
}

extern "C" void emdevif_testEntry(void)
{
    for (const auto& [test_name, test_func] : emdevif_test_list) {
        if (!inIgnoreNameList(test_name)) {
            if (test_func != nullptr) {
                test_func();
            }
        }
    }
}
