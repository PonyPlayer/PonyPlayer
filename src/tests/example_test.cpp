//
// Created by ColorsWind on 2022/04/15.
//

#include <gtest/gtest.h>
TEST(example_test, const_double) {
    double res = 1.0;
    ASSERT_NEAR(res, 1.0, 1.0e-11) << "Should near 1.0";
}


TEST(example_test, const_int) {
    int res = 1;
    ASSERT_EQ(res, 1) << "Should equal 1.";
}

TEST(exmaple_test, const_bool) {
    bool res = true;
    ASSERT_TRUE(res) << "Should be TRUE.";
}