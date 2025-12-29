#include <gtest/gtest.h>
#include "list.hpp"

struct ObjectWithExceptions {
    ObjectWithExceptions(int val):val_(val) {
        if(cnt > 3) {
            throw std::runtime_error("ObjectWithExceptions throw exception");
        }
    };
    static size_t cnt = 0;
    int val_;
};

TEST(list_iterator, ctor) {
    list<int>::Node node(42);
    list<int>::list_iterator it(&node);
    EXPECT_EQ(*it, 42);
}

TEST(list_iterator, equality) {
    list<int>::Node node(42);
    list<int>::list_iterator it(&node);
    list<int>::list_iterator it1(&node);
    
    EXPECT_EQ(it, it1);
    
    list<int>::Node node1(24);
    list<int>::list_iterator it2(&node1);
    EXPECT_NE(it, it2);
}

TEST(list, push_back) {
    list<int> list;
    list.push_back(42);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(*list.begin(), 42);
    EXPECT_NE(list.begin(), list.end());
}

TEST(list, ctor) {
    list<int> list(2);
    EXPECT_EQ(list.size(), 2);

    EXPECT_EQ(*list.begin(), 0);
}

TEST(list, ctor_init_list) {
    list<int> list = {1,2,3,4};
    EXPECT_EQ(*list.begin(), 1);
}

TEST(list, iter_increment) {
    list<int> list = {1,2,3,4};
    EXPECT_EQ(list.size(), 4);

    auto begin = list.begin();
    ++begin;
    EXPECT_EQ(*begin, 2);
    ++begin;
    EXPECT_EQ(*begin, 3);
    ++begin;
    EXPECT_EQ(*begin, 4);
}

TEST(list, range_based_for) {
    list<int> list = {1,2,3,4};
    EXPECT_EQ(list.size(), 4);

    std::vector<int> expected;
    for(auto el : list) {
        expected.push_back(el);
    }
    std::vector actual = {1,2,3,4};
    EXPECT_EQ(expected, actual);
}

TEST(list, iter_add) {
    list<int> list = {1,2,3,4};
    EXPECT_EQ(list.size(), 4);

    auto begin = list.begin();
    ++begin;
    EXPECT_EQ(*begin, 2);
    ++begin;
    EXPECT_EQ(*begin, 3);
    ++begin;
    EXPECT_EQ(*begin, 4);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}