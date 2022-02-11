#include <gtest/gtest.h>
#include "rdsl/devector.hpp"
#include <vector>


TEST(ConstructorTest, defaultConstructor) {
    rdsl::devector<int> vec;
    EXPECT_EQ(vec.data(), nullptr);
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_TRUE(vec.empty());
    std::vector<int> v(15, 20);
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
