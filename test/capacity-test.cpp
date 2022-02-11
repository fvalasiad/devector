#include <gtest/gtest.h>
#include "rdsl/devector.hpp"

TEST(CapacityTest, ALL) {
    rdsl::devector<int> vec(10,20);

    EXPECT_EQ(vec.size(), 10);
    EXPECT_FALSE(vec.empty());
    EXPECT_GE(vec.capacity(), 10);
    vec.resize(20, 40);
    EXPECT_EQ(vec.size(), 20);
    EXPECT_GE(vec.capacity(), 20);
    EXPECT_FALSE(vec.empty());

    vec.reserve(40);
    EXPECT_EQ(vec.size(), 20);
    EXPECT_GE(vec.capacity(), 40);
    EXPECT_FALSE(vec.empty());

    vec.shrink_to_fit();
    EXPECT_EQ(vec.size(), 20);
    EXPECT_EQ(vec.capacity(), 20);
    EXPECT_FALSE(vec.empty());

    vec.resize(10, 3);
    EXPECT_EQ(vec.size(), 10);
    EXPECT_GE(vec.capacity(), 10);
    EXPECT_FALSE(vec.empty());

    vec.reserve(5);
    EXPECT_EQ(vec.size(), 10);
    EXPECT_GE(vec.capacity(), 10);
    EXPECT_FALSE(vec.empty());
}