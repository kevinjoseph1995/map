//
// Created by kevin on 2/23/22.
//
#include <gtest/gtest.h>

#include <iostream>

#include "rbtree.h"

TEST(TEST_RB_TREE, TestEmptyOnConstruction)
{
    RBTree<int, int> tree;
    ASSERT_EQ(0, tree.Size());
}

TEST(TEST_RB_TREE, TestDuplicateKey)
{
    RBTree<std::string, int> tree;
    using namespace std::string_literals;
    bool result;
    auto it = tree.end();

    std::tie(it, result) = tree.Insert({"Key1"s, 3666});
    ASSERT_EQ(true, result);

    std::tie(it, result) = tree.Insert({"Key1"s, 466666});
    ASSERT_EQ(false, result);

    std::tie(it, result) = tree.Insert({"Key2"s, 46});
    ASSERT_EQ(true, result);

    std::tie(it, result) = tree.Insert({"Key3"s, 45});
    ASSERT_EQ(true, result);

    std::tie(it, result) = tree.Insert({"Key2"s, 4});
    ASSERT_EQ(false, result);
}

TEST(TEST_RB_TREE, TestIteratorsReturnedFromInset)
{
    RBTree<int, int> tree;
    tree.Insert({11, 45});
    tree.Insert({2, 45});
    tree.Insert({14, 45});
    tree.Insert({1, 45});
    tree.Insert({7, 45});
    tree.Insert({5, 45});
    tree.Insert({8, 45});
    auto [it_4, result_4] = tree.Insert({4, 45});
    tree.Insert({15, 45});

    ASSERT_EQ(it_4->first, 4);
    ASSERT_EQ((++it_4)->first, 5);
    ASSERT_EQ((++it_4)->first, 7);
    ASSERT_EQ((++it_4)->first, 8);
    ASSERT_EQ((++it_4)->first, 11);
    ASSERT_EQ((++it_4)->first, 14);
    ASSERT_EQ((++it_4)->first, 15);
    ASSERT_EQ((++it_4), tree.end());
}

TEST(TEST_RB_TREE, TestTreeSizeQueryOnInsert)
{
    RBTree<std::string, int> tree;
    using namespace std::string_literals;
    tree.Insert({"Key1"s, 3666});
    ASSERT_EQ(1, tree.Size());
    tree.Insert({"Key1"s, 466666});
    ASSERT_EQ(1, tree.Size());
    tree.Insert({"Key2"s, 46});
    ASSERT_EQ(2, tree.Size());
    tree.Insert({"Key3"s, 45});
    ASSERT_EQ(3, tree.Size());
    tree.Insert({"Key2"s, 4});
    ASSERT_EQ(3, tree.Size());
}

TEST(TEST_RB_TREE, TestIteration)
{
    std::vector<std::vector<int>> test_data_table{
        {8, 5, 34, 3, 2, 99, 6, 7},    {10, 9, 8, 7, 3}, {1, 2, -1, 6, 7, 8}, {10, 8, 9, 7, 6, 5, 4, 3}, {1},
        {11, 2, 1, 14, 15, 7, 8, 4, 5}};
    for (auto &test_entry : test_data_table)
    {
        RBTree<int, int> tree;
        for (auto key : test_entry)
        {
            static constexpr int VALUE = 1;
            tree.Insert({key, VALUE});
        }
        std::sort(test_entry.begin(), test_entry.end());
        auto tree_iterator = tree.begin();
        for (auto key : test_entry)
        {
            ASSERT_EQ(key, (tree_iterator++)->first); // Checks if we get keys out in
                                                      // sorted order out the RBTree
                                                      // when iterating through it
        }
    }
}

TEST(TEST_RB_TREE, TestReverseIteration)
{
    std::vector<std::vector<int>> test_data_table{
        {8, 5, 34, 3, 2, 99, 6, 7},    {10, 9, 8, 7, 3}, {1, 2, -1, 6, 7, 8}, {10, 8, 9, 7, 6, 5, 4, 3}, {1},
        {11, 2, 1, 14, 15, 7, 8, 4, 5}};
    for (auto &test_entry : test_data_table)
    {
        RBTree<int, int> tree;
        for (auto key : test_entry)
        {
            static constexpr int VALUE = 1;
            tree.Insert({key, VALUE});
        }
        std::sort(test_entry.begin(), test_entry.end(), std::greater<>());
        auto tree_iterator = --tree.end();
        for (auto key : test_entry)
        {
            ASSERT_EQ(key,
                      (tree_iterator--)->first); // Checks if we get keys out in
                                                 // reverse-sorted order out the
                                                 // RBTree when iterating through it
        }
    }
}

TEST(TEST_RB_TREE, TestInsertingOnlyMovalbleObjects)
{
    struct MovableObject
    {
        MovableObject() = default;
        MovableObject(const MovableObject &) = delete;
        MovableObject(MovableObject &) = delete;
        MovableObject &operator=(const MovableObject &) = delete;

        MovableObject &operator=(MovableObject &&) noexcept
        {
            // std::cout << "Move assignment operator called\n";
            return *this;
        };
        MovableObject(MovableObject &&) noexcept {
            // std::cout << "Move constructor called\n";
        };

        int dummy_data{};
    };
    using namespace std::string_literals;
    RBTree<std::string, MovableObject> tree;
    auto [it, result] = tree.Insert({"KEY"s, MovableObject()});
    ASSERT_EQ(true, result);
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
