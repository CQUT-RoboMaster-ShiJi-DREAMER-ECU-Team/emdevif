/**
 * @file c_feature_test.c
 * @brief C 语言特性测试
 */

#include <stddef.h>

#include "emdevif_test_framework.h"

TEST_SUIT(CFeatureTest)
{
    TEST_CASE_BEGIN(StructDefaultInit)
    {
        struct S {
            const char* a;
            const char* b;
        };

        struct S s1 = {"a", "b"};  // NOLINT
        ASSERT_TRUE(s1.a != NULL && s1.b != NULL, "");
        ASSERT_STREQ(s1.a, "a", "");
        ASSERT_STREQ(s1.b, "b", "");

        struct S s2 = {.a = "a", .b = "b"};  // NOLINT
        ASSERT_TRUE(s2.a != NULL && s2.b != NULL, "");
        ASSERT_STREQ(s2.a, "a", "");
        ASSERT_STREQ(s2.b, "b", "");

        struct S s3 = {.a = "a", "b"};  // NOLINT
        ASSERT_TRUE(s3.a != NULL && s3.b != NULL, "");
        ASSERT_STREQ(s3.a, "a", "");
        ASSERT_STREQ(s3.b, "b", "");

        // NOLINTNEXTLINE
        struct S s4 = {
            .a = "a",
        };
        ASSERT_TRUE(s4.a != NULL, "");
        ASSERT_TRUE(s4.b == NULL, "");
        ASSERT_STREQ(s4.a, "a", "");

        // NOLINTNEXTLINE
        struct S s5 = {
            .b = "b",
        };
        ASSERT_TRUE(s5.a == NULL, "");
        ASSERT_TRUE(s5.b != NULL, "");
        ASSERT_STREQ(s5.b, "b", "");
    }
    TEST_CASE_END();
}

void cFeatureTest(void)
{
    RUN_SUIT(CFeatureTest);
}
