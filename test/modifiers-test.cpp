#include <gtest/gtest.h>
#include "rdsl/devector.hpp"

struct input_it: public std::iterator<std::input_iterator_tag, int> {
    input_it(const input_it&) = default;
    input_it(input_it&&) noexcept = default;
    input_it& operator=(const input_it&) = default;
    input_it& operator=(input_it&&) = default;

    explicit input_it(int state = 0): state(state) {}

    int state;

    int operator*() { return state; }
    input_it& operator++() { ++state; return *this; }
    input_it operator++(int) { ++state; return input_it(state - 1); }

    bool operator!=(const input_it& other) { return state != other.state; }
};

TEST(ModifiersTest, AssignTest) {
    rdsl::devector<int> vec(10,20);
    
    rdsl::devector<int> vec1;

    vec1.assign(vec.begin(), vec.size());
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
    
    vec.insert(vec.begin() + 4, temp.begin(), temp.size());
    EXPECT_EQ(vec.size(), 29);
    EXPECT_FALSE(vec.empty());
    EXPECT_GE(vec.capacity(), 29);

    vec.insert(vec.begin() + 9, 3, 2);
    EXPECT_EQ(vec.size(), 32);
    EXPECT_FALSE(vec.empty());
    EXPECT_GE(vec.capacity(), 32);

    rdsl::devector<int> result{23, 54, 94, 432, 72, 53, 4324, 653, 123, 2, 2, 2, 3, 65, 543, 13, 5432, 954,4321};
    for(int i = 0; i < 19; i = i + 1){
        EXPECT_EQ(vec[i], result[i]);
    }
    for(int i = 19; i < 32; i = i + 1){
        EXPECT_EQ(vec[i], 20);
    }

    vec.erase(vec.begin());
    vec.erase(vec.end() - 1);
    vec.erase(vec.begin() + 3, vec.end());
    EXPECT_EQ(vec.size(), 3);
    EXPECT_FALSE(vec.empty());
    EXPECT_GE(vec.capacity(), 3);

    EXPECT_EQ(vec[0], 54);
    EXPECT_EQ(vec[1], 94);
    EXPECT_EQ(vec[2], 432);

    vec.emplace_back(4);
    vec.emplace_front(9);
    vec.emplace(vec.begin() + 2, 13);

    EXPECT_EQ(vec.size(), 6);
    EXPECT_FALSE(vec.empty());
    EXPECT_GE(vec.capacity(), 6);

    EXPECT_EQ(vec[0], 9);
    EXPECT_EQ(vec[1], 54);
    EXPECT_EQ(vec[2], 13);
    EXPECT_EQ(vec[3], 94);
    EXPECT_EQ(vec[4], 432);
    EXPECT_EQ(vec[5], 4);

    vec.insert(vec.begin() + 2, input_it(), input_it(10));
    
    EXPECT_EQ(vec.size(), 16);
    EXPECT_FALSE(vec.empty());
    EXPECT_GE(vec.capacity(), 16);

    EXPECT_EQ(vec[0], 9);
    EXPECT_EQ(vec[1], 54);
    EXPECT_EQ(vec[2], 0);
    EXPECT_EQ(vec[3], 1);
    EXPECT_EQ(vec[4], 2);
    EXPECT_EQ(vec[5], 3);
    EXPECT_EQ(vec[6], 4);
    EXPECT_EQ(vec[7], 5);
    EXPECT_EQ(vec[8], 6);
    EXPECT_EQ(vec[9], 7);
    EXPECT_EQ(vec[10], 8);
    EXPECT_EQ(vec[11], 9);
    EXPECT_EQ(vec[12], 13);
    EXPECT_EQ(vec[13], 94);
    EXPECT_EQ(vec[14], 432);
    EXPECT_EQ(vec[15], 4);
}
