#include <gtest/gtest.h>
#include "rdsl/devector.hpp"

TEST(AccessTest, ALL) {
    rdsl::devector<int> vec{5 , 123, 543, 954, 123, 32, 54};

    EXPECT_EQ(vec[0], 5);
    EXPECT_EQ(vec[1], 123);
    EXPECT_EQ(vec[2], 543);
    EXPECT_EQ(vec[3], 954);
    EXPECT_EQ(vec[4], 123);
    EXPECT_EQ(vec[5], 32);
    EXPECT_EQ(vec[6], 54);

    EXPECT_EQ(vec.at(0), 5);
    EXPECT_EQ(vec.at(1), 123);
    EXPECT_EQ(vec.at(2), 543);
    EXPECT_EQ(vec.at(3), 954);
    EXPECT_EQ(vec.at(4), 123);
    EXPECT_EQ(vec.at(5), 32);
    EXPECT_EQ(vec.at(6), 54);

    try{
        vec.at(7);
        EXPECT_TRUE(false);
    }catch(const std::out_of_range& e){}

    EXPECT_EQ(vec.front(), 5);
    EXPECT_EQ(vec.back(), 54);
}