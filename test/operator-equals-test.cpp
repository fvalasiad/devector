#include <gtest/gtest.h>
#include "rdsl/devector.hpp"

TEST(OperatorEqualsTest, copyOperatorEquals) {
    rdsl::devector<int> vec0(10,20);

    rdsl::devector<int> vec1;
    vec1 = vec0;
    ASSERT_NE(vec1.data(), nullptr);
    EXPECT_EQ(vec1.size(), 10);
    EXPECT_GE(vec1.capacity(), 10);
    ASSERT_FALSE(vec0.empty());

    for(const int& i: vec1){
        ASSERT_EQ(i, 20);
    }

    rdsl::devector<int> vec2(1000,432432);
    vec1 = vec2;
    ASSERT_NE(vec1.data(), nullptr);
    EXPECT_EQ(vec1.size(), 1000);
    EXPECT_GE(vec1.capacity(), 1000);
    ASSERT_FALSE(vec0.empty());

    for(const int& i: vec1){
        ASSERT_EQ(i, 432432);
    }

    vec2.assign(500, 343);
    vec1 = vec2;
    ASSERT_NE(vec1.data(), nullptr);
    EXPECT_EQ(vec1.size(), 500);
    EXPECT_GE(vec1.capacity(), 500);
    ASSERT_FALSE(vec0.empty());

    for(const int& i: vec1){
        ASSERT_EQ(i, 343);
    }
}

TEST(OperatorEqualsTest, moveOperatorEquals) {
    rdsl::devector<int> vec0(10,20);

    rdsl::devector<int> vec1;
    vec1 = std::move(vec0);
    ASSERT_NE(vec1.data(), nullptr);
    EXPECT_EQ(vec1.size(), 10);
    EXPECT_GE(vec1.capacity(), 10);
    ASSERT_FALSE(vec1.empty());

    for(const int& i: vec1){
        ASSERT_EQ(i, 20);
    }

    EXPECT_EQ(vec0.data(), nullptr);
    EXPECT_EQ(vec0.size(), 0);
    EXPECT_EQ(vec0.capacity(), 0);
    EXPECT_TRUE(vec0.empty());
}

TEST(OperatorEqualsTest, ilOperatorEquals) {
    rdsl::devector<int> vec0;
    vec0 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    ASSERT_NE(vec0.data(), nullptr);
    EXPECT_EQ(vec0.size(), 10);
    EXPECT_GE(vec0.capacity(), 10);
    ASSERT_FALSE(vec0.empty());

    for(int i = 0; i < 10; i = i + 1){
        ASSERT_EQ(i, vec0[i]);
    }
}