/**
 * @file type_traits_test.cpp
 * @author DuYicheng
 * @date 2025-10-05
 * @brief 类型特征测试
 */

#include <utility>
#include <tuple>
#include <array>

#include "emdevif_test_framework.h"

import emdevif.typeTraits;

namespace emdevif::static_assert_test::is_consteval_test {

static constexpr auto a = 0;
static auto b = 0;

static_assert(emdevif::is_consteval([] { return a; }));
static_assert(!emdevif::is_consteval([] { return b; }));

static consteval auto funcA() {}
static constexpr auto funcB() {}
static auto funcC() {}

static_assert(emdevif::is_consteval([] { return funcA(); }));
static_assert(emdevif::is_consteval([] { return funcB(); }));
static_assert(!emdevif::is_consteval([] { return funcC(); }));
static_assert(emdevif::is_consteval([] { funcA(); }));
static_assert(emdevif::is_consteval([] { funcB(); }));
static_assert(!emdevif::is_consteval([] { funcC(); }));

class C
{
public:
    consteval auto funcA() const {}
    constexpr auto funcB() const {}
    auto funcC() const {}
};

static constexpr C c1;
static_assert(emdevif::is_consteval([] { return c1.funcA(); }));
static_assert(emdevif::is_consteval([] { return c1.funcB(); }));
static_assert(!emdevif::is_consteval([] { return c1.funcC(); }));

static C c2;
static_assert(emdevif::is_consteval([] { return c2.funcA(); }));
static_assert(emdevif::is_consteval([] { return c2.funcB(); }));
static_assert(!emdevif::is_consteval([] { return c2.funcC(); }));

}  // namespace emdevif::static_assert_test::is_consteval_test

namespace emdevif::static_assert_test::tuple_style_test {

namespace struct_style {

struct S2 {
    int a;
    float b;
};
using T2 = std::tuple<int, float>;
using P2 = std::pair<int, float>;

struct S3 {
    int a;
    float b;
    char c;
};
using T3 = std::tuple<int, float, char>;

static_assert(emdevif::is_tuple_like_v<T2>);
static_assert(emdevif::is_tuple_like_v<T3>);
static_assert(emdevif::is_tuple_like_v<P2>);
static_assert(!emdevif::is_tuple_like_v<S2>);
static_assert(!emdevif::is_tuple_like_v<S3>);

static_assert(emdevif::is_tuple_style_v<T2>);
static_assert(emdevif::is_tuple_style_v<T3>);
static_assert(emdevif::is_tuple_style_v<P2>);
static_assert(emdevif::is_tuple_style_v<S2>);
static_assert(emdevif::is_tuple_style_v<S3>);

static_assert(emdevif::is_same_tuple_style_v<T2, T2>);
static_assert(emdevif::is_same_tuple_style_v<T2, P2>);
static_assert(emdevif::is_same_tuple_style_v<T2, S2>);
static_assert(emdevif::is_same_tuple_style_v<T3, T3>);
static_assert(emdevif::is_same_tuple_style_v<T3, S3>);

}  // namespace struct_style

namespace array_style {

using A2 = std::array<int, 2>;
using P2 = std::pair<int, int>;
using T2 = std::tuple<int, int>;

using A3 = std::array<char, 3>;
using T3 = std::tuple<char, char, char>;

static_assert(emdevif::is_tuple_like_v<T2>);
static_assert(emdevif::is_tuple_like_v<T3>);
static_assert(emdevif::is_tuple_like_v<P2>);
static_assert(!emdevif::is_tuple_like_v<A2>);
static_assert(!emdevif::is_tuple_like_v<A3>);

static_assert(emdevif::is_tuple_style_v<T2>);
static_assert(emdevif::is_tuple_style_v<T3>);
static_assert(emdevif::is_tuple_style_v<P2>);
static_assert(emdevif::is_tuple_style_v<A2>);
static_assert(emdevif::is_tuple_style_v<A3>);

static_assert(emdevif::is_same_tuple_style_v<T2, T2>);
static_assert(emdevif::is_same_tuple_style_v<T2, P2>);
static_assert(emdevif::is_same_tuple_style_v<T2, A2>);
static_assert(emdevif::is_same_tuple_style_v<T3, T3>);
static_assert(emdevif::is_same_tuple_style_v<T3, A3>);

}  // namespace array_style

}  // namespace emdevif::static_assert_test::tuple_style_test

TEST_SUIT(TupleCastTest)
{
    TEST_CASE_BEGIN(tuple_to_aggregate_StructStyleTest)
    {
        using namespace emdevif::static_assert_test::tuple_style_test::struct_style;
        do {
            const T2 t2{42, 3.14f};
            const auto s2 = emdevif::tuple_to_aggregate<S2>(t2);
            INT_EXPECT_EQ(std::get<0>(t2), s2.a);
            FP_EXPECT_EQ(std::get<1>(t2), s2.b);
        } while (false);
        do {
            P2 t2{42, 3.14f};
            auto s2 = emdevif::tuple_to_aggregate<S2>(t2);
            INT_EXPECT_EQ(t2.first, s2.a);
            FP_EXPECT_EQ(t2.second, s2.b);
        } while (false);
        do {
            constexpr T2 t2{42, 3.14f};
            constexpr auto s2 = emdevif::tuple_to_aggregate<S2>(t2);
            INT_EXPECT_EQ(std::get<0>(t2), s2.a);
            FP_EXPECT_EQ(std::get<1>(t2), s2.b);
        } while (false);

        do {
            T3 t3{42, 114.514f, 'c'};
            auto s3 = emdevif::tuple_to_aggregate<S3>(t3);
            INT_EXPECT_EQ(std::get<0>(t3), s3.a);
            FP_EXPECT_EQ(std::get<1>(t3), s3.b);
            INT_EXPECT_EQ(std::get<2>(t3), s3.c);
        } while (false);
        do {
            constexpr T3 t3{42, 114.514f, 'c'};
            constexpr auto s3 = emdevif::tuple_to_aggregate<S3>(t3);
            INT_EXPECT_EQ(std::get<0>(t3), s3.a);
            FP_EXPECT_EQ(std::get<1>(t3), s3.b);
            INT_EXPECT_EQ(std::get<2>(t3), s3.c);
        } while (false);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(aggregate_to_tuple_StructStyleTest)
    {
        using namespace emdevif::static_assert_test::tuple_style_test::struct_style;
        do {
            const S2 s2{1234, 114.514f};
            const auto t2 = emdevif::aggregate_to_tuple(s2);
            INT_EXPECT_EQ(std::get<0>(t2), s2.a);
            FP_EXPECT_EQ(std::get<1>(t2), s2.b);
        } while (false);
        do {
            const S2 s2{-531154, 1919.810f};
            const P2 t2 = emdevif::aggregate_to_pair(s2);
            INT_EXPECT_EQ(t2.first, s2.a);
            FP_EXPECT_EQ(t2.second, s2.b);
        } while (false);
        do {
            constexpr S2 s2{51425, 1535.515f};
            constexpr auto t2 = emdevif::aggregate_to_tuple(s2);
            INT_EXPECT_EQ(std::get<0>(t2), s2.a);
            FP_EXPECT_EQ(std::get<1>(t2), s2.b);
        } while (false);

        do {
            S3 s3{42, 114.514f, 'c'};
            auto t3 = emdevif::aggregate_to_tuple(s3);
            INT_EXPECT_EQ(std::get<0>(t3), s3.a);
            FP_EXPECT_EQ(std::get<1>(t3), s3.b);
            INT_EXPECT_EQ(std::get<2>(t3), s3.c);
        } while (false);
        do {
            constexpr S3 s3{42, 114.514f, 'c'};
            constexpr auto t3 = emdevif::aggregate_to_tuple(s3);
            INT_EXPECT_EQ(std::get<0>(t3), s3.a);
            FP_EXPECT_EQ(std::get<1>(t3), s3.b);
            INT_EXPECT_EQ(std::get<2>(t3), s3.c);
        } while (false);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(tuple_to_aggregate_ArrayStyleTest)
    {
        using namespace emdevif::static_assert_test::tuple_style_test::array_style;
        do {
            const T2 t2{42, -114514};
            const auto s2 = emdevif::tuple_to_aggregate<A2>(t2);
            INT_EXPECT_EQ(std::get<0>(t2), s2[0]);
            INT_EXPECT_EQ(std::get<1>(t2), s2[1]);
        } while (false);
        do {
            P2 t2{42, 325425};
            auto s2 = emdevif::tuple_to_aggregate<A2>(t2);
            INT_EXPECT_EQ(t2.first, s2[0]);
            INT_EXPECT_EQ(t2.second, s2[1]);
        } while (false);
        do {
            constexpr T2 t2{42, 5432342};
            constexpr auto s2 = emdevif::tuple_to_aggregate<A2>(t2);
            INT_EXPECT_EQ(std::get<0>(t2), s2[0]);
            INT_EXPECT_EQ(std::get<1>(t2), s2[1]);
        } while (false);

        do {
            T3 t3{42, ' ', 'c'};
            auto s3 = emdevif::tuple_to_aggregate<A3>(t3);
            INT_EXPECT_EQ(std::get<0>(t3), s3[0]);
            INT_EXPECT_EQ(std::get<1>(t3), s3[1]);
            INT_EXPECT_EQ(std::get<2>(t3), s3[2]);
        } while (false);
        do {
            constexpr T3 t3{42, 'k', '\n'};
            constexpr auto s3 = emdevif::tuple_to_aggregate<A3>(t3);
            INT_EXPECT_EQ(std::get<0>(t3), s3[0]);
            INT_EXPECT_EQ(std::get<1>(t3), s3[1]);
            INT_EXPECT_EQ(std::get<2>(t3), s3[2]);
        } while (false);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(aggregate_to_tuple_ArrayStyleTest)
    {
        using namespace emdevif::static_assert_test::tuple_style_test::array_style;
        do {
            const A2 a2{1234, -114514};
            const auto t2 = emdevif::aggregate_to_tuple(a2);
            INT_EXPECT_EQ(std::get<0>(t2), a2[0]);
            INT_EXPECT_EQ(std::get<1>(t2), a2[1]);
        } while (false);
        do {
            A2 a2{1234, -114514};
            auto t2 = emdevif::aggregate_to_pair(a2);
            INT_EXPECT_EQ(t2.first, a2[0]);
            INT_EXPECT_EQ(t2.second, a2[1]);
        } while (false);
        do {
            constexpr A2 a2{1234, -114514};
            constexpr auto t2 = emdevif::aggregate_to_tuple(a2);
            INT_EXPECT_EQ(std::get<0>(t2), a2[0]);
            INT_EXPECT_EQ(std::get<1>(t2), a2[1]);
        } while (false);

        do {
            A3 a3{'d', ' ', 'c'};
            auto t3 = emdevif::aggregate_to_tuple(a3);
            INT_EXPECT_EQ(std::get<0>(t3), a3[0]);
            INT_EXPECT_EQ(std::get<1>(t3), a3[1]);
            INT_EXPECT_EQ(std::get<2>(t3), a3[2]);
        } while (false);
        do {
            constexpr A3 a3{'\n', 'a', '\\'};
            constexpr auto t3 = emdevif::aggregate_to_tuple(a3);
            INT_EXPECT_EQ(std::get<0>(t3), a3[0]);
            INT_EXPECT_EQ(std::get<1>(t3), a3[1]);
            INT_EXPECT_EQ(std::get<2>(t3), a3[2]);
        } while (false);
    }
    TEST_CASE_END();
}

void typeTraitsTest()
{
    RUN_SUIT(TupleCastTest);
}
