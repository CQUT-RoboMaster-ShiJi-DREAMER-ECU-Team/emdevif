/**
 * @file InitOnce_test.cpp
 * @brief 单次初始化类测试
 */

#include <algorithm>

#include "emdevif_test_framework.h"

#ifdef EMDEVIF_USE_MODULES
import emdevif.util.init_once;
import emdevif.core.error_handler;
#else
    #include "emdevif/util/init_once.hpp"
    #include "emdevif/core/error_handler.hpp"
#endif

static TEST_SUIT(InitOnceBasicTest)
{
    TEST_CASE_BEGIN(InitOnceBasicTest);
    {
        using emdevif::InitOnce;

        InitOnce<int> a;
        EXPECT_TRUE(a == 0);
        EXPECT_TRUE(a() == 0);

        a = 114514;
        EXPECT_TRUE(a == 114514);

        a = 1;
        EXPECT_TRUE(a() == 114514 && a != 1);

        auto error_code = a.init(2472);
        EXPECT_TRUE(error_code == emdevif::ErrorCode::AlreadyExists);
        EXPECT_TRUE(a == 114514 && a() != 2472);

        InitOnce<float> f;
        FP_EXPECT_EQ(f, 0.0f);

        error_code = f.init(2.3f);
        EXPECT_TRUE(error_code == emdevif::ErrorCode::Success);
        FP_EXPECT_EQ(f(), 2.3f);

        f = 114514.1919810f;
        FP_EXPECT_EQ(f(), 2.3f);

        error_code = f.init(4235.2);
        EXPECT_TRUE(error_code == emdevif::ErrorCode::AlreadyExists);
        FP_EXPECT_EQ(f(), 2.3f);

        InitOnce<short> b(8);
        EXPECT_TRUE(b == 8);

        b = 1;
        EXPECT_TRUE(b == 8 && b() == 8);

        const auto c = static_cast<InitOnce<signed char>>(2);
        EXPECT_TRUE(c == 2 && c() == 2);

        constexpr InitOnce d{2.3f};
        FP_EXPECT_EQ(d, 2.3f);
    }
    TEST_CASE_END();
}

constinit emdevif::InitOnce global1{5};
constinit emdevif::InitOnce<int> global2{};
constinit const emdevif::InitOnce global3{5.4};
constexpr emdevif::InitOnce global4{5};
constinit const emdevif::InitOnce<int> global5;

static TEST_SUIT(InitOnceGlobalTest)
{
    TEST_CASE_BEGIN(InitOnceGlobalTest);
    {
        global1 = 1;
        EXPECT_TRUE(global1() == 5);

        auto err = global1.init(23415);
        EXPECT_TRUE(global1 == 5 && err == emdevif::ErrorCode::AlreadyExists);

        EXPECT_TRUE(global2 == 0);
        global2 = 2;
        EXPECT_TRUE(global2 == 2);

        err = global2.init(23415);
        EXPECT_TRUE(global2 == 2 && err == emdevif::ErrorCode::AlreadyExists);

        FP_EXPECT_EQ(global3(), 5.4);

        EXPECT_TRUE(global4 == 5);

        EXPECT_TRUE(global5 == 0);
    }
    TEST_CASE_END();
}

// only for suit MoveTest
bool test_allocate_failed = false;

void allocateFailed()
{
    test_allocate_failed = true;
}
// todo Release 模式下会有很多警告
static TEST_SUIT(InitOnceMoveTest)
{
    class Test final
    {
    public:
        Test()
        {
            p_data = new int[5];
            if (p_data == nullptr) {
                allocateFailed();
            }
        }

        Test(const std::initializer_list<int> data)
        {
            p_data = new int[5];
            if (p_data == nullptr) {
                allocateFailed();
            }
            auto size = data.size();
            if (size > 5) {
                size = 5;
            }
            std::copy_n(data.begin(), size, p_data);
        }

        Test(const Test& other)
        {
            p_data = new int[5];
            if (p_data == nullptr) {
                allocateFailed();
            }
            std::copy_n(other.p_data, 5, p_data);
        }

        Test& operator=(const Test& other)
        {
            if (this != &other) {
                if (p_data == nullptr) {
                    p_data = new int[5];
                    if (p_data == nullptr) {
                        allocateFailed();
                    }
                }
                std::copy_n(other.p_data, 5, p_data);
            }
            return *this;
        }

        Test(Test&& other) noexcept
        {
            p_data = other.p_data;
            other.p_data = nullptr;
        }

        Test& operator=(Test&& other) noexcept
        {
            if (this == &other) {
                return *this;
            }

            if (p_data != nullptr) {
                delete p_data;
            }

            p_data = other.p_data;
            other.p_data = nullptr;

            return *this;
        }

        ~Test()
        {
            delete p_data;
            p_data = nullptr;
        }

        bool operator==(const Test& other) const
        {
            return std::equal(p_data, p_data + 5, other.p_data, other.p_data + 5);
        }

        [[nodiscard]] int* data()
        {
            return p_data;
        }

        [[nodiscard]] const int* data() const
        {
            return p_data;
        }

    private:
        int* p_data = nullptr;
    };

    test_allocate_failed = false;
    TEST_CASE_BEGIN(ClassMoveTest);
    {
        const Test a{1, 2, 3, 4, 5};
        ASSERT_FALSE(test_allocate_failed, "");
        Test b{a};

        ASSERT_FALSE(test_allocate_failed, "");

        EXPECT_TRUE(a == b);

        Test c(std::move(b));
        ASSERT_FALSE(test_allocate_failed, "");
        EXPECT_TRUE(c == a && b.data() == nullptr);

        b = a;
        EXPECT_TRUE(b == a);

        Test d{};
        ASSERT_FALSE(test_allocate_failed, "");
        d = a;
        EXPECT_TRUE(d == a);

        d = std::move(b);
        ASSERT_FALSE(test_allocate_failed, "");
        EXPECT_TRUE(d == a && b.data() == nullptr);
    }
    TEST_CASE_END();

    test_allocate_failed = false;
    TEST_CASE_BEGIN(InitOnceMoveTest);

    {
        using emdevif::InitOnce;

        InitOnce<Test> b;
        ASSERT_FALSE(test_allocate_failed, "");
        const Test a{1, 2, 3, 4, 5};
        ASSERT_FALSE(test_allocate_failed, "");
        const Test dummy{114514, 1919, 810, 24, 251};
        ASSERT_FALSE(test_allocate_failed, "");

        Test dummy_2(dummy);
        ASSERT_FALSE(test_allocate_failed, "");

        b = a;
        ASSERT_FALSE(test_allocate_failed, "");
        EXPECT_TRUE(b() == a);
        b = dummy;
        ASSERT_FALSE(test_allocate_failed, "");
        EXPECT_TRUE(b() == a);
        b = std::move(dummy_2);
        ASSERT_FALSE(test_allocate_failed, "");
        EXPECT_TRUE(b() == a && dummy_2.data() != nullptr);

        InitOnce<Test> c, d;
        ASSERT_FALSE(test_allocate_failed, "");
        c = a;
        ASSERT_FALSE(test_allocate_failed, "");
        EXPECT_TRUE(c() == a);
        c = dummy;
        ASSERT_FALSE(test_allocate_failed, "");
        EXPECT_TRUE(c() == a);

        d = std::move(c);
        ASSERT_FALSE(test_allocate_failed, "");
        EXPECT_TRUE(d() == a && c().data() == nullptr);
        d = dummy;
        ASSERT_FALSE(test_allocate_failed, "");
        EXPECT_TRUE(d() == a && c().data() == nullptr);

        InitOnce<Test> e(b);
        ASSERT_FALSE(test_allocate_failed, "");
        EXPECT_TRUE(e() == b() && e() == a);
    }
    TEST_CASE_END();
}

void initOnceTest()
{
    RUN_SUIT(InitOnceBasicTest);
    RUN_SUIT(InitOnceGlobalTest);
    RUN_SUIT(InitOnceMoveTest);
}