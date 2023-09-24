#include <gtest/gtest.h>
#include "jsondb.h"

using namespace jsondb;

template <typename T>
class NumericComparisonUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TYPED_TEST_SUITE_P(NumericComparisonUtilsTest);

template <typename T>
class StringComparisonUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TYPED_TEST_SUITE_P(StringComparisonUtilsTest);

TYPED_TEST_P(NumericComparisonUtilsTest, EqTest) {
    TypeParam val1 = 5;
    TypeParam val2 = 5;
    EXPECT_TRUE(ComparisonUtils<TypeParam>::Eq(val1, val2));
    EXPECT_FALSE(ComparisonUtils<TypeParam>::Eq(val1, val2 + 1));
}

TYPED_TEST_P(NumericComparisonUtilsTest, GtTest) {
    TypeParam val1 = 5;
    TypeParam val2 = 3;
    EXPECT_TRUE(ComparisonUtils<TypeParam>::Gt(val1, val2));
    EXPECT_FALSE(ComparisonUtils<TypeParam>::Gt(val1, val1));
}

TYPED_TEST_P(NumericComparisonUtilsTest, GteTest) {
    TypeParam val1 = 5;
    TypeParam val2 = 5;
    TypeParam val3 = 4;
    EXPECT_TRUE(ComparisonUtils<TypeParam>::Gte(val1, val2));
    EXPECT_TRUE(ComparisonUtils<TypeParam>::Gte(val1, val3));
    EXPECT_FALSE(ComparisonUtils<TypeParam>::Gte(val2, val1 + 1));
}

TYPED_TEST_P(NumericComparisonUtilsTest, InTest) {
    TypeParam val = 3;
    std::vector<TypeParam> arr = {1, 2, 3, 4, 5};
    EXPECT_TRUE(ComparisonUtils<TypeParam>::In(val, arr));
    EXPECT_FALSE(ComparisonUtils<TypeParam>::In(val, std::vector<TypeParam>()));
}

TYPED_TEST_P(NumericComparisonUtilsTest, LtTest) {
    TypeParam val1 = 3;
    TypeParam val2 = 5;
    EXPECT_TRUE(ComparisonUtils<TypeParam>::Lt(val1, val2));
    EXPECT_FALSE(ComparisonUtils<TypeParam>::Lt(val1, val1));
}

TYPED_TEST_P(NumericComparisonUtilsTest, LteTest) {
    TypeParam val1 = 5;
    TypeParam val2 = 5;
    TypeParam val3 = 6;
    
    EXPECT_TRUE(ComparisonUtils<TypeParam>::Lte(val1, val2));
    EXPECT_TRUE(ComparisonUtils<TypeParam>::Lte(val1, val3));
    EXPECT_FALSE(ComparisonUtils<TypeParam>::Lte(val1 + 1, val2));
}

TYPED_TEST_P(NumericComparisonUtilsTest, NeTest) {
    TypeParam val1 = 3;
    TypeParam val2 = 2;
    std::vector<TypeParam> arr = {1, 2, 4, 5};
    EXPECT_TRUE(ComparisonUtils<TypeParam>::Ne(val1, arr));
    EXPECT_FALSE(ComparisonUtils<TypeParam>::Ne(val2, arr));
}

TYPED_TEST_P(StringComparisonUtilsTest, EqTest) {
    const TypeParam val1 = "hello";
    const TypeParam val2 = "hello";
    const TypeParam val3 = "bye";
    EXPECT_TRUE(ComparisonUtils<TypeParam>::Eq(val1, val2));
    EXPECT_FALSE(ComparisonUtils<TypeParam>::Eq(val1, val3));
}

TYPED_TEST_P(StringComparisonUtilsTest, InTest) {
    TypeParam val = "hello";
    std::vector<TypeParam> arr = {"hello", "hi", "good morning"};
    EXPECT_TRUE(ComparisonUtils<TypeParam>::In(val, arr));
    EXPECT_FALSE(ComparisonUtils<TypeParam>::In(val, std::vector<TypeParam>()));
}

TYPED_TEST_P(StringComparisonUtilsTest, NeTest) {
    TypeParam val1 = "bye";
    TypeParam val2 = "hi";
    std::vector<TypeParam> arr = {"hello", "hi", "good morning"};
    EXPECT_TRUE(ComparisonUtils<TypeParam>::Ne(val1, arr));
    EXPECT_FALSE(ComparisonUtils<TypeParam>::Ne(val2, arr));
}

REGISTER_TYPED_TEST_SUITE_P(NumericComparisonUtilsTest,
    EqTest,
    GtTest,
    GteTest,
    InTest,
    LtTest,
    LteTest,
    NeTest
);

REGISTER_TYPED_TEST_SUITE_P(StringComparisonUtilsTest,
    EqTest,
    InTest,
    NeTest
);

typedef ::testing::Types<int, float> NumberTypes;
typedef ::testing::Types<std::string, const char*> StringTypes;

INSTANTIATE_TYPED_TEST_SUITE_P(Numbers, NumericComparisonUtilsTest, NumberTypes);
INSTANTIATE_TYPED_TEST_SUITE_P(Strings, StringComparisonUtilsTest, StringTypes);
