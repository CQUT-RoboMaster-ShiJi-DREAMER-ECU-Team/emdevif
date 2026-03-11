/**
 * @file heap_test.cpp
 * @brief 堆内存测试
 */

#include <memory>
#include <initializer_list>
#include <array>
#include <vector>
#include <algorithm>

#include "emdevif_test_framework.h"

#include "heap_usage_checker.hpp"

#ifdef EMDEVIF_USE_MODULES
import emdevif.system.heap;
#else
    #include "emdevif/system/heap.hpp"
#endif

using namespace emdevif;

TEST_SUIT(HeapTest)
{
    RECORD_MEMORY_INFO();

    TEST_CASE_BEGIN(BasicTest)
    {
        auto a = heap::construct<int>(std::nothrow);
        ASSERT_TRUE(a != nullptr, "");

        heap::destruct(a);
        ASSERT_TRUE(a == nullptr, "");
        heap::destruct(a);  // 可以安全的删除 nullptr（会直接返回，不会非法内存访问）

        a = heap::construct<int>(std::nothrow, 114514);
        ASSERT_TRUE(a != nullptr, "");
        INT_EXPECT_EQ(*a, 114514);

        heap::destruct(a);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(ArrayTest)
    {
        auto a = heap::construct<int[5]>(std::nothrow);
        ASSERT_TRUE(a != nullptr, "");

        heap::destruct(a);
        ASSERT_TRUE(a == nullptr, "");
        heap::destruct(a);  // 可以安全的删除 nullptr（会直接返回，不会非法内存访问）

        a = heap::construct<int[5]>(std::nothrow, 1, 2, 3, 4, 5);
        ASSERT_TRUE(a != nullptr, "");
        auto is_pass = true;
        for (const auto i : {1, 2, 3, 4, 5}) {
            if ((*a)[i - 1] != i) {
                is_pass = false;
                break;
            }
        }
        EXPECT_TRUE(is_pass);

        heap::destruct(a);
        ASSERT_TRUE(a == nullptr, "");

        auto b = heap::construct<std::array<std::array<int, 4>, 3>>(
            std::nothrow,
            std::array<std::array<int, 4>, 3>{{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}}});
        ASSERT_TRUE(b != nullptr, "");

        is_pass = true;
        [&] {
            for (const auto i : {0, 1, 2}) {
                for (const auto j : {1, 2, 3, 4}) {
                    if ((*b)[i][j - 1] != i * 4 + j) {
                        is_pass = false;
                        return;
                    }
                }
            }
        }();
        EXPECT_TRUE(is_pass);

        heap::destruct(b);
        ASSERT_TRUE(b == nullptr, "");
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(ClassTest)
    {
        class C final
        {
        public:
            C() noexcept : value(heap::construct<int>(std::nothrow)) {}

            explicit C(const int& other) noexcept : value(heap::construct<int>(std::nothrow, other)) {}

            C(const C& other) noexcept : C(*other.value) {}

            C(C&& other) noexcept : value(other.value)
            {
                other.value = nullptr;
            }

            ~C()
            {
                heap::destruct(value);
            }

            const int& operator()() const
            {
                return *value;
            }

            int& operator()()
            {
                return *value;
            }

            [[nodiscard]] int* getPtr() const
            {
                return value;
            }

        private:
            int* value;
        };

        auto c = heap::construct<C>(std::nothrow);
        ASSERT_TRUE(c != nullptr, "");

        heap::destruct(c);
        ASSERT_TRUE(c == nullptr, "");

        c = heap::construct<C>(std::nothrow, -1919810);
        ASSERT_TRUE(c != nullptr, "");
        INT_EXPECT_EQ(c->operator()(), -1919810);

        do {
            C c2 = *c;
            INT_EXPECT_EQ(c2.operator()(), -1919810);
        } while (false);
        do {
            auto c2 = heap::construct<C>(std::nothrow, *c);
            ASSERT_TRUE(c2 != nullptr, "");
            INT_EXPECT_EQ(c2->operator()(), -1919810);

            heap::destruct(c2);
            ASSERT_TRUE(c2 == nullptr, "");
        } while (false);

        ASSERT_TRUE(c != nullptr, "");

        do {
            C c3 = std::move(*c);
            ASSERT_TRUE(c != nullptr, "");
            ASSERT_TRUE(c->getPtr() == nullptr, "");

            heap::destruct(c);  // 此时，c 的资源被移动了，因此不可再使用
            ASSERT_TRUE(c == nullptr, "");

            ASSERT_TRUE(c3.getPtr() != nullptr, "");
            ASSERT_TRUE(c3.operator()() == -1919810, "");
        } while (false);
        ASSERT_TRUE(c == nullptr, "");

        c = heap::construct<C>(std::nothrow, 114514);
        ASSERT_TRUE(c != nullptr, "");
        ASSERT_TRUE(c->getPtr() != nullptr, "");
        INT_EXPECT_EQ(c->operator()(), 114514);
        do {
            auto c3 = heap::construct<C>(std::nothrow, std::move(*c));
            ASSERT_TRUE(c3 != nullptr, "");
            ASSERT_TRUE(c3->getPtr() != nullptr, "");
            INT_EXPECT_EQ(c3->operator()(), 114514);
            ASSERT_TRUE(c != nullptr, "");
            ASSERT_TRUE(c->getPtr() == nullptr, "");

            heap::destruct(c3);
        } while (false);
        heap::destruct(c);
        ASSERT_TRUE(c == nullptr, "");
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(UniquePtrTest)
    {
        do {
            auto v = heap::make_unique<int>(std::nothrow, -114514);
            ASSERT_TRUE(v != nullptr, "");
            INT_EXPECT_EQ(*v, -114514);
        } while (false);

        do {
            auto v = heap::make_unique<int[6]>(std::nothrow, 1, 2, 3, 4, 5, 6);
            ASSERT_TRUE(v != nullptr, "");
            bool pass = true;
            for (const auto i : {1, 2, 3, 4, 5, 6}) {
                if ((*v)[i - 1] != i) {
                    pass = false;
                    break;
                }
            }
            EXPECT_TRUE(pass);
        } while (false);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(AllocatorTest)
    {
        std::vector<int, heap::Allocator<int>> v{1, 2, 3, 4, 5};
        constexpr std::array a{1, 2, 3, 4, 5};
        EXPECT_TRUE(v.size() == 5)->MESSAGE("v.size() = %zu", v.size());
        EXPECT_TRUE(std::equal(v.begin(), v.end(), a.begin(), a.end()));

        v.emplace_back(-1);
        v.push_back(14135);
        constexpr std::array b{1, 2, 3, 4, 5, -1, 14135};
        EXPECT_TRUE(v.size() == 7)->MESSAGE("v.size() = %zu", v.size());
        EXPECT_TRUE(std::equal(v.begin(), v.end(), b.begin(), b.end()));

        v.pop_back();
        constexpr std::array c{1, 2, 3, 4, 5, -1};
        EXPECT_TRUE(std::equal(v.begin(), v.end(), c.begin(), c.end()));
        EXPECT_TRUE(v.size() == 6)->MESSAGE("v.size() = %zu", v.size());
    }
    TEST_CASE_END();

    CHECK_MEMORY_LEAK();
}

void heapTest()
{
    RUN_SUIT(HeapTest);
}
