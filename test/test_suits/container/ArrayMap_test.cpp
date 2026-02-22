/**
 * @file ArrayMap_test.cpp
 * @brief 数组键值对测试
 */

#include "emdevif_test_framework.h"

#include <cstring>

#include <string_view>
#include <array>

import emdevif.core.integer_suffix;
import emdevif.data_container.array_map;

namespace emdevif::test::static_map {

static constexpr auto map1 = emdevif::makeStaticMap<int, char>({{1, 'a'}, {2, 'b'}, {114514, 'k'}});

static_assert(map1.find(1919810) == map1.cend());
static_assert(map1.at(9) == nullptr);
static_assert(*map1.at(2) == 'b');
static_assert(map1[2] == 'b');
static_assert(map1[114514] == 'k');
static_assert(map1.maxSize() == map1.size());
static_assert(map1.size() == 3);
static_assert(map1.maxSizeByte() == map1.sizeByte());
static_assert(map1.sizeByte() == 3 * sizeof(char));

static constexpr auto map2 =
    emdevif::makeStaticMap<std::string_view, int>({{"a", 1}, {"abc", 2}, {"cdef", 3}, {"qwert", 1244}});

static_assert(map2.find("none") == map2.cend());
static_assert(map2["a"] == 1);
static_assert(map2["qwert"] == 1244);
static_assert(map2.maxSize() == map2.size());
static_assert(map2.size() == 4);
static_assert(map2.maxSizeByte() == map2.sizeByte());
static_assert(map2.sizeByte() == 4 * sizeof(int));

}  // namespace emdevif::test::static_map

TEST_SUIT(ArrayMapTest)
{
    TEST_CASE_BEGIN(ConstructTest)
    {
        using namespace emdevif;

        ArrayMap<std::string_view, int, 4> map1{{{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}}};
        UINT_EXPECT_EQ(map1.size(), 4);
        UINT_EXPECT_EQ(map1.maxSize(), 4);
        UINT_EXPECT_EQ(map1.maxSizeByte(), 4 * sizeof(int));
        UINT_EXPECT_EQ(map1.sizeByte(), 4 * sizeof(int));
        INT_EXPECT_EQ(map1["three"], 3);
        INT_EXPECT_EQ(map1["four"], 4);
        EXPECT_TRUE(map1.isFull());
        EXPECT_FALSE(map1.isEmpty());

        for (auto cnt = 1; auto& e : map1) {
            const auto& [k, v] = e;
            INT_EXPECT_EQ(cnt, v);

            ++cnt;
        }

        decltype(map1) map2 = map1;
        for (auto cnt = 1; auto& e : map2) {
            auto [k, v] = e;
            INT_EXPECT_EQ(cnt, v);

            ++cnt;
        }

        // 由于 map1 是由 map2 拷贝构造而来，而 map2 的测试检测是通过循环变量实现的，
        // 因此 map1 的“改”操作在这里进行
        map1["three"] = 333;
        map1["four"] = 444;
        INT_EXPECT_EQ(map1["three"], 333);
        INT_EXPECT_EQ(map1["four"], 444);

        auto map3 =
            makeArrayMap<std::string_view, int, 10>({{"qwe", 23}, {"rty", 12}, {"3245", 3245}, {"afsfew", 6311}});
        UINT_EXPECT_EQ(map3.size(), 4);
        UINT_EXPECT_EQ(map3.sizeByte(), 4 * sizeof(int));
        UINT_EXPECT_EQ(map3.maxSize(), 10);
        UINT_EXPECT_EQ(map3.maxSizeByte(), 10 * sizeof(int));
        EXPECT_FALSE(map3.isFull());
        EXPECT_FALSE(map3.isEmpty());

        INT_EXPECT_EQ(map3["qwe"], 23);
        INT_EXPECT_EQ(map3["rty"], 12);
        INT_EXPECT_EQ(map3["3245"], 3245);
        INT_EXPECT_EQ(map3["afsfew"], 6311);

        auto map3_key_list = std::to_array<std::string_view>({"qwe", "rty", "3245", "afsfew"});
        auto key_list = map3_key_list.begin();

        const auto map3_it_d = std::distance(map3.begin(), map3.end());
        const auto key_list_d = std::distance(map3_key_list.begin(), map3_key_list.end());
        UINT_ASSERT_EQ(map3_it_d, key_list_d, "");

        for (auto map_it = map3.begin(); map_it != map3.end(); ++map_it, ++key_list) {
            const auto& [k, v] = *map_it;

            EXPECT_STREQ(k.data(), key_list->data());
        }
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(InsertEraseTest)
    {
        using namespace emdevif;

        auto map =
            makeArrayMap<std::string_view, int, 10>({{"qwe", 23}, {"rty", 12}, {"3245", 3245}, {"afsfew", 6311}});
        UINT_EXPECT_EQ(map.size(), 4);
        UINT_EXPECT_EQ(map.sizeByte(), 4 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));

        INT_EXPECT_EQ(map["qwe"], 23);
        INT_EXPECT_EQ(map["rty"], 12);
        INT_EXPECT_EQ(map["3245"], 3245);
        INT_EXPECT_EQ(map["afsfew"], 6311);

        map.insert({"three", 3});
        UINT_EXPECT_EQ(map.size(), 5);
        UINT_EXPECT_EQ(map.sizeByte(), 5 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));
        INT_EXPECT_EQ(map["three"], 3);

        map.insert({"four", 4});
        UINT_EXPECT_EQ(map.size(), 6);
        UINT_EXPECT_EQ(map.sizeByte(), 6 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));
        INT_EXPECT_EQ(map["four"], 4);

        auto ret = map.erase("rty");
        UINT_EXPECT_EQ(1, ret);
        UINT_EXPECT_EQ(map.size(), 5);
        UINT_EXPECT_EQ(map.sizeByte(), 5 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));
        EXPECT_TRUE(map.find("rty") == map.end());

        ret = map.erase("four");
        UINT_EXPECT_EQ(1, ret);
        UINT_EXPECT_EQ(map.size(), 4);
        UINT_EXPECT_EQ(map.sizeByte(), 4 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));
        EXPECT_TRUE(map.find("four") == map.end());

        ret = map.erase("114514");
        UINT_EXPECT_EQ(0, ret);
        UINT_EXPECT_EQ(map.size(), 4);
        UINT_EXPECT_EQ(map.sizeByte(), 4 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(EraseIfTest)
    {
        using namespace emdevif;

        auto map = makeArrayMap<int, char>({{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}, {5, 'e'}});

        // 擦除 Key 为偶数的项
        const auto count = erase_if(map, [](const auto& item) {
            auto const& [key, value] = item;
            return (key % 2) != 0;
        });

        UINT_EXPECT_EQ(count, 3);
        UINT_EXPECT_EQ(map.size(), 2);
        UINT_EXPECT_EQ(map.sizeByte(), 2 * sizeof(char));

        constexpr auto expect_map_list = std::to_array<std::pair<int, char>>({{2, 'b'}, {4, 'd'}});
        UINT_ASSERT_EQ(map.size(), expect_map_list.size(), "");

        auto expect_index = 0_zu;
        for (auto& e : map) {
            const auto& [key, value] = e;
            const auto& [expect_k, expect_v] = expect_map_list[expect_index];

            INT_EXPECT_EQ(expect_k, key);
            INT_EXPECT_EQ(expect_v, value);

            ++expect_index;
        }
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(KeyEqualTParamTest)
    {
        struct constCharPtrEq {
            bool operator()(char const* l, char const* r) const
            {
                return std::strcmp(l, r) == 0;
            }
        };

        using namespace emdevif;

        auto map = makeArrayMap<const char*, int, 10, constCharPtrEq>(
            {{"qwe", 23}, {"rty", 12}, {"3245", 3245}, {"afsfew", 6311}});
        static_assert(std::is_same_v<decltype(map.key_eq()), constCharPtrEq>);
        EXPECT_TRUE(map.key_eq()("Hello world!", "Hello world!"));

        UINT_EXPECT_EQ(map.size(), 4);
        UINT_EXPECT_EQ(map.sizeByte(), 4 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));

        INT_EXPECT_EQ(map["qwe"], 23);
        INT_EXPECT_EQ(map["rty"], 12);
        INT_EXPECT_EQ(map["3245"], 3245);
        INT_EXPECT_EQ(map["afsfew"], 6311);

        map.insert({"three", 3});
        UINT_EXPECT_EQ(map.size(), 5);
        UINT_EXPECT_EQ(map.sizeByte(), 5 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));
        INT_EXPECT_EQ(map["three"], 3);

        map.insert({"four", 4});
        UINT_EXPECT_EQ(map.size(), 6);
        UINT_EXPECT_EQ(map.sizeByte(), 6 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));
        INT_EXPECT_EQ(map["four"], 4);

        auto ret = map.erase("rty");
        UINT_EXPECT_EQ(1, ret);
        UINT_EXPECT_EQ(map.size(), 5);
        UINT_EXPECT_EQ(map.sizeByte(), 5 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));
        EXPECT_TRUE(map.find("rty") == map.end());

        ret = map.erase("four");
        UINT_EXPECT_EQ(1, ret);
        UINT_EXPECT_EQ(map.size(), 4);
        UINT_EXPECT_EQ(map.sizeByte(), 4 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));
        EXPECT_TRUE(map.find("four") == map.end());

        ret = map.erase("114514");
        UINT_EXPECT_EQ(0, ret);
        UINT_EXPECT_EQ(map.size(), 4);
        UINT_EXPECT_EQ(map.sizeByte(), 4 * sizeof(int));
        UINT_EXPECT_EQ(map.maxSize(), 10);
        UINT_EXPECT_EQ(map.maxSizeByte(), 10 * sizeof(int));
    }
    TEST_CASE_END();
}

void mapTest()
{
    RUN_SUIT(ArrayMapTest);
}
