#include <gtest/gtest.h>
#include "list.hpp"

struct ObjectWithExceptions {
    ObjectWithExceptions(int val):val_(val) {
        if(cnt > 3) {
            throw std::runtime_error("ObjectWithExceptions throw exception");
        }
    };
    inline static size_t cnt = 0;
    int val_;
};

TEST(list_iterator, ctor) {
    list<int>::Node node(42);
    list<int>::list_iterator it(node.asBase());
    EXPECT_EQ(*it, 42);
}

TEST(list_iterator, equality) {
    list<int>::Node node(42);
    list<int>::list_iterator it(node.asBase());
    list<int>::list_iterator it1(node.asBase());
    
    EXPECT_EQ(it, it1);
    
    list<int>::Node node1(24);
    list<int>::list_iterator it2(node1.asBase());
    EXPECT_NE(it, it2);
}

TEST(list, default_ctor) {
    list<int> list;
//     EXPECT_EQ(list.begin(), list.end());
    EXPECT_EQ(list.size(), 0);
}

TEST(list, insert_like_push_back) {
    list<int> list;
    list.insert(list.begin(), 42);
    
    EXPECT_EQ(*list.begin(), 42);
    EXPECT_EQ(list.size(), 1);
    EXPECT_NE(list.begin(), list.end());
}

TEST(list, push_back) {
    list<int> list;
    list.push_back(1);
    list.push_back(2);

    EXPECT_EQ(*list.begin(), 1);
}
TEST(list, push_front) {
    list<int> list;
    list.push_front(1);
    list.push_front(2);

    EXPECT_EQ(*list.begin(), 2);
}

TEST(list, iter_increment_decrement) {
    list<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    
    auto begin = list.begin();
    ++begin;
    EXPECT_EQ(*begin, 2);
    ++begin;
    EXPECT_EQ(*begin, 3);
    ++begin;
    EXPECT_EQ(*begin, 4);
    --begin;
    EXPECT_EQ(*begin, 3);
    --begin;
    EXPECT_EQ(*begin, 2);
}

// TEST(list, pop_back) {
//     list<int> list = {1,2,3,4};
//     list.pop_back();

//     EXPECT_EQ(list.size(), 3);
//     for(auto el : list) {
//         std::cout << el << std::endl;
//     }
//     auto it = list.end() -= 2;
//     EXPECT_EQ(*it, 3);
// }

// TEST(list, ctor) {
//     list<int> list(2);
//     EXPECT_EQ(list.size(), 2);

//     EXPECT_EQ(*list.begin(), 0);
// }

// TEST(list, ctor_init_list) {
//     list<int> list = {1,2,3,4};
//     EXPECT_EQ(*list.begin(), 1);
// }

// TEST(list, iter_increment_decrement) {
//     list<int> list = {1,2,3,4};
//     EXPECT_EQ(list.size(), 4);

//     auto begin = list.begin();
//     ++begin;
//     EXPECT_EQ(*begin, 2);
//     ++begin;
//     EXPECT_EQ(*begin, 3);
//     ++begin;
//     EXPECT_EQ(*begin, 4);
//     --begin;
//     EXPECT_EQ(*begin, 3);
//     --begin;
//     EXPECT_EQ(*begin, 2);
// }

// TEST(list, iter_add) {
//     list<int> list = {1,2,3,4};
//     EXPECT_EQ(list.size(), 4);
    
//     auto it = list.begin() += 1;
//     EXPECT_EQ(*it, 2);
//     it += 2;
//     EXPECT_EQ(*it, 4);
//     it -= 1;
//     EXPECT_EQ(*it, 3);
// }

// TEST(list, range_based_for) {
//     list<int> list = {1,2,3,4};
//     EXPECT_EQ(list.size(), 4);

//     std::vector<int> expected;
//     for(auto el : list) {
//         expected.push_back(el);
//     }
//     std::vector actual = {1,2,3,4};
//     EXPECT_EQ(expected, actual);
// }

// TEST(list, strong_exception_garantee_ctor) {
//     list<ObjectWithExceptions> list(4);
// }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}