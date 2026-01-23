#include <gtest/gtest.h>
#include <vector>
#include <list>
#include "list.hpp"

struct ObjectWithExceptions {
    ObjectWithExceptions() {
        // std::cout << cnt << std::endl;
        if(cnt > 2) {
            throw std::runtime_error("ObjectWithExceptions throw exception");
        }
        ++cnt;
    };
    inline static size_t cnt = 0;
};

TEST(list_iterator, ctor) {
    exp::list<int>::Node node(42);
    exp::list<int>::iterator it(node.asBase());
    EXPECT_EQ(*it, 42);
}

TEST(list_iterator, equality) {
    exp::list<int>::Node node(42);
    exp::list<int>::iterator it(node.asBase());
    exp::list<int>::iterator it1(node.asBase());
    
    EXPECT_EQ(it, it1);
    
    exp::list<int>::Node node1(24);
    exp::list<int>::iterator it2(node1.asBase());
    EXPECT_NE(it, it2);
}

TEST(list_iterator, const_nonconst) {
    EXPECT_FALSE(std::is_const_v<std::remove_reference_t<exp::list<int>::iterator::reference>>);
    EXPECT_TRUE(std::is_const_v<std::remove_reference_t<exp::list<int>::const_iterator::reference>>);

    EXPECT_FALSE(std::is_const_v<exp::list<int>::iterator::pointer>);
    EXPECT_TRUE(std::is_const_v<exp::list<int>::const_iterator::pointer>);
}

TEST(list, default_ctor) {
    exp::list<int> list;
    EXPECT_EQ(list.begin(), list.end());
    EXPECT_EQ(list.size(), 0);
}

TEST(list, insert_like_push_back) {
    exp::list<int> list;
    list.insert(list.begin(), 42);
    
    EXPECT_EQ(*list.begin(), 42);
    EXPECT_EQ(list.size(), 1);
    EXPECT_NE(list.begin(), list.end());
}


TEST(list, push_back) {
    exp::list<int> list;
    list.push_back(1);
    list.push_back(2);

    EXPECT_EQ(*list.begin(), 1);
}
TEST(list, push_front) {
    exp::list<int> list;
    list.push_front(1);
    list.push_front(2);

    EXPECT_EQ(*list.begin(), 2);
}

TEST(list, iter_increment_decrement) {
    exp::list<int> list;
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

TEST(list, iter_advance) {
    exp::list<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);

    EXPECT_EQ(*(list.begin() += 1), 2);
    EXPECT_EQ(*(list.begin() += 2), 3);
    EXPECT_EQ(*(list.begin() += 3), 4);
    auto it = list.begin() += 3;
    EXPECT_EQ(*(it -= 1), 3);
    EXPECT_EQ(*(it -= 2), 1);
}

TEST(list, ostream_operator) {
    exp::list<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);

    std::stringstream ss;
    for(auto& el : list) {
        ss << el;
    }
    EXPECT_EQ(ss.str(), std::string("1234"));
}

TEST(list, n_ctor) {
    exp::list<int> list(3);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(*list.begin(), 0);
}

TEST(list, ctor_init_list) {
    exp::list<int> list = {1,2,3,4};
    EXPECT_EQ(*list.begin(), 1);
}

TEST(list, range_based_for) {
    exp::list<int> list = {1,2,3,4};
    EXPECT_EQ(list.size(), 4);

    std::vector<int> expected;
    for(auto el : list) {
        expected.push_back(el);
    }
    std::vector actual = {1,2,3,4};
    EXPECT_EQ(expected, actual);
}

TEST(list, erase) {
    exp::list<int> list = {1,2,3,4};

    list.erase(list.begin());
    EXPECT_EQ(*list.begin(), 2);

    list.erase(list.begin());
    EXPECT_EQ(*list.begin(), 3);

    list.erase(list.begin());
    EXPECT_EQ(*list.begin(), 4);
    EXPECT_EQ(list.size(), 1);
}

TEST(list, pop_front) {
    exp::list<int> list = {1,2,3,4};

    list.pop_front();
    EXPECT_EQ(*list.begin(), 2);

    list.pop_front();
    EXPECT_EQ(*list.begin(), 3);

    list.pop_front();
    EXPECT_EQ(*list.begin(), 4);
    EXPECT_EQ(list.size(), 1);
}

TEST(list, pop_back) {
    exp::list<int> list = {1,2,3,4};

    list.pop_back();
    EXPECT_EQ(*std::prev(list.end()), 3);

    list.pop_back();
    EXPECT_EQ(*std::prev(list.end()), 2);

    list.pop_back();
    EXPECT_EQ(*list.begin(), 1);
    EXPECT_EQ(*std::prev(list.end()), 1);
}

TEST(list, equal) {
    exp::list<int> list1 = {1,2,3,4};
    exp::list<int> list2 = {1,2,3,4};
    exp::list<int> list3 = {1,5,3,4};

    EXPECT_TRUE(list1 == list2);
    EXPECT_TRUE(list1 != list3);
}

TEST(list, strong_exception_garantee_push_back) {
    exp::list<ObjectWithExceptions> list;
    list.push_back({});
    list.push_back({});
    list.push_back({});
    
    EXPECT_THROW(list.push_back({}), std::runtime_error);

    EXPECT_EQ(list.size(), 3);

    ObjectWithExceptions::cnt = 0;
}

// TEST(list, strong_exception_garantee_ctor) {
//     exp::list<ObjectWithExceptions> list(3);

//     EXPECT_EQ(list.size(), 0);
//     ObjectWithExceptions::cnt = 0;
// }
// TEST(list, strong_exception_garantee_init_ctor) {
//     exp::list<ObjectWithExceptions> list = {{}, {}, {}};

//     EXPECT_EQ(list.size(), 0);
//     ObjectWithExceptions::cnt = 0;
// }

// TEST(list, it_ctor) {
//     std::vector vec {1,2,3,4};

//     exp::list list(vec.begin(), vec.end());

//     exp::list list1 {1,2,3,4};
//     EXPECT_EQ(list, list1);
// }

// TEST(list, copy_ctor) {
//     exp::list list {1,2,3,4};

//     auto list1 = list;

//     EXPECT_EQ(list, list1);
// }

// TEST(list, move_ctor) {
//     exp::list list {1,2,3,4};
//     exp::list ref_list {1,2,3,4};

//     auto list1 = std::move(list);

//     EXPECT_EQ(list1, ref_list);
// }

TEST(zip_iterator, ctor_and_deref) {
    std::vector vec = {1,2,3,4};
    std::list list = {5,6,7,8};
    const std::vector cvec = {0,2,4,6};
    std::vector<bool> bvec = {true, false, false, true};

    auto it1 = vec.begin();
    const auto it2 = list.begin();
    
    auto zit = zip_iterator(it1, it2, cvec.begin(), bvec.begin());
    auto [val1, val2, val3, val4] = *zit;
    EXPECT_EQ(val1, 1);
    EXPECT_EQ(val2, 5);
    EXPECT_EQ(val3, 0);
    EXPECT_TRUE(val4);
}

TEST(zip_iterator, increment) {
    std::vector vec = {1,2,3,4};
    std::list list = {5,6,7,8};

    auto zit = zip_iterator(vec.begin(), list.begin());
++zit;
    auto [val1, val2] = *zit;
    EXPECT_EQ(val1, 2);
    EXPECT_EQ(val2, 6);
    zit++;
    auto [val3, val4] = *zit;
    EXPECT_EQ(val3, 3);
    EXPECT_EQ(val4, 7);
}

TEST(zip_iterator, equal) {
    std::vector vec = {1,2,3,4};
    std::list list = {5,6,7,8};
    
    auto zit = zip_iterator(vec.begin(), list.begin());
    auto zit2 = zip_iterator(vec.begin(), list.begin());
    auto zit1 = zip_iterator(vec.end(), list.end());
    
    ++zit2;

    EXPECT_EQ(zit, zit);
    EXPECT_NE(zit, zit1);
    EXPECT_NE(zit, zit2);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}