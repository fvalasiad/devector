#include <gtest/gtest.h>
#include "rdsl/devector.hpp"

TEST(IteratorTest, Normal) {
    rdsl::devector<int> vec0{1, 2, 3};

    auto it = vec0.begin();

    EXPECT_EQ(*it, 1);
    EXPECT_EQ(it[1], 2);
    EXPECT_EQ(it[2], 3);
    EXPECT_EQ(*++it, 2);
    EXPECT_EQ(it[-1], 1);
    it -= 1;
    EXPECT_EQ(it[0], 1);
    it += 2;
    EXPECT_EQ(it[0], 3);
    EXPECT_EQ(*it--, 3);
    EXPECT_EQ(*it, 2);
    
    auto it1 = vec0.end();
    
    EXPECT_EQ(it1 - it, 2);

    //==, !=
    it = vec0.begin();
    EXPECT_NE(it, it1);
    it = vec0.end();
    EXPECT_EQ(it, it1);

    //*
    it = vec0.begin();
    EXPECT_EQ(*it, 1);

    //<, >, <=, >=
    EXPECT_LT(it, it1);
    EXPECT_GT(it1, it);
    EXPECT_LE(it, it1);
    EXPECT_GE(it1, it);

    it = vec0.end();
    EXPECT_LE(it, it1);
    EXPECT_GE(it, it1);
}

TEST(IteratorTest, Reverse) {
    rdsl::devector<int> vec0{1, 2, 3};

    auto it = vec0.rbegin();
    auto it1 = vec0.rend();

    EXPECT_EQ(*it++, 3);
    EXPECT_EQ(*it++, 2);
    EXPECT_EQ(*it++, 1);

    EXPECT_EQ(*--it1, 1);
    EXPECT_EQ(*--it1, 2);
    EXPECT_EQ(*--it1, 3);
}