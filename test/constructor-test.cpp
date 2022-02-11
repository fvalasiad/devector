#include <gtest/gtest.h>
#include "rdsl/devector.hpp"
#include <vector>


TEST(ConstructorTest, defaultConstructor) {
    rdsl::devector<int> vec;
    EXPECT_EQ(vec.data(), nullptr);
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_TRUE(vec.empty());
}

TEST(ConstructorTest, fillConstructor) {
    rdsl::devector<int> vec0(10);
    EXPECT_NE(vec0.data(), nullptr);
    EXPECT_EQ(vec0.size(), 10);
    EXPECT_GE(vec0.capacity(), 10);
    EXPECT_FALSE(vec0.empty());

    rdsl::devector<int> vec1(15, 20);
    EXPECT_NE(vec1.data(), nullptr);
    EXPECT_EQ(vec1.size(), 15);
    EXPECT_GE(vec1.capacity(), 15);
    EXPECT_FALSE(vec1.empty());

    for(const int& i: vec1){
        EXPECT_EQ(i, 20);
    }
}

TEST(ConstructorTest, rangeConstructor) {
    rdsl::devector<int> vec0(10,20);

    rdsl::devector<int> vec1(vec0.begin(), vec0.end(), vec0.size());
    ASSERT_NE(vec1.data(), nullptr);
    EXPECT_EQ(vec1.size(), 10);
    EXPECT_GE(vec1.capacity(), 10);
    ASSERT_FALSE(vec0.empty());

    for(const int& i: vec1){
        ASSERT_EQ(i, 20);
    }

    rdsl::devector<int> vec2(vec1.begin(), vec1.end());
    ASSERT_NE(vec2.data(), nullptr);
    EXPECT_EQ(vec2.size(), 10);
    EXPECT_GE(vec2.capacity(), 10);
    ASSERT_FALSE(vec2.empty());

    for(const int& i: vec2){
        ASSERT_EQ(i, 20);
    }
}

TEST(ConstructorTest, copyConstructor) {
    rdsl::devector<int> vec0(10,20);

    rdsl::devector<int> vec1(vec0);
    ASSERT_NE(vec1.data(), nullptr);
    EXPECT_EQ(vec1.size(), 10);
    EXPECT_GE(vec1.capacity(), 10);
    ASSERT_FALSE(vec0.empty());

    for(const int& i: vec1){
        ASSERT_EQ(i, 20);
    }
}

TEST(ConstructorTest, moveConstructor) {
    rdsl::devector<int> vec0(10,20);

    rdsl::devector<int> vec1(std::move(vec0));
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

TEST(ConstructorTest, ilConstructor) {
    rdsl::devector<int> vec0{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    ASSERT_NE(vec0.data(), nullptr);
    EXPECT_EQ(vec0.size(), 10);
    EXPECT_GE(vec0.capacity(), 10);
    ASSERT_FALSE(vec0.empty());

    for(int i = 0; i < 10; i = i + 1){
        ASSERT_EQ(i, vec0[i]);
    }
}
