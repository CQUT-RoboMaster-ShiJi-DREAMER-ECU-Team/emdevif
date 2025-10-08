/**
 * @file type_traits_test.cpp
 * @author DuYicheng
 * @date 2025-10-05
 * @brief 类型特征测试
 */

import emdevif.typeTraits;

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
