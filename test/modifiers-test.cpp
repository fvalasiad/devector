#include <gtest/gtest.h>
#include "rdsl/devector.hpp"
#include <vector>

TEST(ModifiersTest, AssignTest) {
    rdsl::devector<int> vec(10,20);
    
    rdsl::devector<int> vec1;

    vec1.assign(vec.begin(), vec.end(), vec.size());
    EXPECT_EQ(vec1.size(), 10);
    EXPECT_FALSE(vec1.empty());
    EXPECT_GE(vec1.capacity(), 10);
    for(const int& i: vec1){
        EXPECT_EQ(i, 20);
    }

    vec1.assign(30, 23);
    EXPECT_EQ(vec1.size(), 30);
    EXPECT_FALSE(vec1.empty());
    EXPECT_GE(vec1.capacity(), 30);
    for(const int& i: vec1){
        EXPECT_EQ(i, 23);
    }

    vec1.assign({3, 4, 5, 6, 7});
    EXPECT_EQ(vec1.size(), 5);
    EXPECT_FALSE(vec1.empty());
    EXPECT_GE(vec1.capacity(), 5);
    
    EXPECT_EQ(vec1[0], 3);
    EXPECT_EQ(vec1[1], 4);
    EXPECT_EQ(vec1[2], 5);
    EXPECT_EQ(vec1[3], 6);
    EXPECT_EQ(vec1[4], 7);
}

TEST(ModifiersTest, PushPopBackFrontTest) {
    rdsl::devector<int> vec;

    vec.push_front(10);
    vec.push_back(20);
    vec.push_front(30);
    vec.push_front(40);
    vec.push_back(50);

    EXPECT_EQ(vec.size(), 5);
    EXPECT_FALSE(vec.empty());
    EXPECT_GE(vec.capacity(), 5);

    EXPECT_EQ(vec[0], 40);
    EXPECT_EQ(vec[1], 30);
    EXPECT_EQ(vec[2], 10);
    EXPECT_EQ(vec[3], 20);
    EXPECT_EQ(vec[4], 50);

    vec.pop_front();
    vec.pop_front();
    vec.pop_back();

    EXPECT_EQ(vec.size(), 2);
    EXPECT_FALSE(vec.empty());
    EXPECT_GE(vec.capacity(), 2);

    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
}

TEST(ModifiersTest, InsertEraseTest) {
    rdsl::devector<int> vec{3, 65, 543, 13, 5432, 954, 4321};

    vec.insert(vec.end(), 13, 20);
    EXPECT_EQ(vec.size(), 20);
    EXPECT_FALSE(vec.empty());
    EXPECT_GE(vec.capacity(), 20);
    for(int i = 7; i < 20; i = i + 1){
        EXPECT_EQ(vec[i], 20);
    }

    vec.insert(vec.begin(), {23, 54, 94, 432});
    EXPECT_EQ(vec.size(), 24);
    EXPECT_FALSE(vec.empty());
    EXPECT_GE(vec.capacity(), 24);

    EXPECT_EQ(vec[0], 23);
    EXPECT_EQ(vec[1], 54);
    EXPECT_EQ(vec[2], 94);
    EXPECT_EQ(vec[3], 432);

    rdsl::devector<int> temp{72,53,4324,653,123};
    
    vec.insert(vec.begin() + 4, temp.begin(), temp.end(), temp.size());
    EXPECT_EQ(vec.size(), 29);
    EXPECT_FALSE(vec.empty());
    EXPECT_GE(vec.capacity(), 29);

    rdsl::devector<int> result{23, 54, 94, 432, 72, 53, 4324, 654, 123, 3, 65, 543, 13, 5432, 954,4321};
    for(int i = 0; i < 16; i = i + 1){
        EXPECT_EQ(vec[i], result[i]);
    }
    for(int i = 16; i < 29; i = i + 1){
        EXPECT_EQ(vec[i], 20);
    }
}