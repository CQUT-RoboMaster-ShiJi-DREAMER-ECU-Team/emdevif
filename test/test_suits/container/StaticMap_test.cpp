/**
 * @file StaticMap_test.cpp
 * @author DuYicheng
 * @date 2025-09-04
 * @brief 静态键值对测试
 */

#include <string_view>

import emdevif.container.Map;

using emdevif::StaticMap;

static constexpr auto map1 = emdevif::makeStaticMap<int, char>({{1, 'a'}, {2, 'b'}, {114514, 'k'}});

static_assert(map1.find(1919810) == map1.cend());
static_assert(map1.at(9) == nullptr);
static_assert(*map1.at(2) == 'b');
static_assert(map1[2] == 'b');
static_assert(map1[114514] == 'k');

static constexpr auto map2 =
    emdevif::makeStaticMap<std::string_view, int>({{"a", 1}, {"abc", 2}, {"cdef", 3}, {"qwert", 1244}});

static_assert(map2.find("none") == map2.cend());
static_assert(map2["a"] == 1);
static_assert(map2["qwert"] == 1244);
