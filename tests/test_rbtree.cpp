//
// Created by kevin on 2/23/22.
//
#include <gtest/gtest.h>

#include <random>

#include "rbtree.h"

TEST(TEST_RB_TREE, TestEmptyOnConstruction) {
  RBTree<int, int> tree;
  ASSERT_EQ(0, tree.Size());
}

TEST(TEST_RB_TREE, TestDuplicateKey) {
  RBTree<std::string, int> tree;
  using namespace std::string_literals;
  ASSERT_EQ(true, tree.Insert({"Key1"s, 3666}));
  ASSERT_EQ(false, tree.Insert({"Key1"s, 466666}));

  ASSERT_EQ(true, tree.Insert({"Key2"s, 46}));
  ASSERT_EQ(true, tree.Insert({"Key3"s, 45}));
  ASSERT_EQ(false, tree.Insert({"Key2"s, 4}));
}

TEST(TEST_RB_TREE, TestTreeSizeQueryOnInsert) {
  RBTree<std::string, int> tree;
  using namespace std::string_literals;
  ASSERT_EQ(true, tree.Insert({"Key1"s, 3666}));
  ASSERT_EQ(1, tree.Size());
  ASSERT_EQ(false, tree.Insert({"Key1"s, 466666}));
  ASSERT_EQ(1, tree.Size());
  ASSERT_EQ(true, tree.Insert({"Key2"s, 46}));
  ASSERT_EQ(2, tree.Size());
  ASSERT_EQ(true, tree.Insert({"Key3"s, 45}));
  ASSERT_EQ(3, tree.Size());
  ASSERT_EQ(false, tree.Insert({"Key2"s, 4}));
  ASSERT_EQ(3, tree.Size());
}

TEST(TEST_RB_TREE, TestIteration) {
  std::vector<std::vector<int>> test_data_table{{8, 5, 34, 3, 2, 99, 6, 7},
                                                {10, 9, 8, 7, 3},
                                                {1, 2, -1, 6, 7, 8},
                                                {10, 8, 9, 7, 6, 5, 4, 3},
                                                {1},
                                                {11, 2, 1, 14, 15, 7, 8, 4, 5}};
  for (auto& test_entry : test_data_table) {
    RBTree<int, int> tree;
    for (auto key : test_entry) {
      static constexpr int VALUE = 1;
      tree.Insert({key, VALUE});
    }
    std::sort(test_entry.begin(), test_entry.end());
    auto tree_iterator = tree.begin();
    for (auto key : test_entry) {
      ASSERT_EQ(key, (tree_iterator++)->first);  // Checks if we get keys out in
                                                 // sorted order out the RBTree
                                                 // when iterating through it
    }
  }
}

TEST(TEST_RB_TREE, TestReverseIteration) {
  std::vector<std::vector<int>> test_data_table{{8, 5, 34, 3, 2, 99, 6, 7},
                                                {10, 9, 8, 7, 3},
                                                {1, 2, -1, 6, 7, 8},
                                                {10, 8, 9, 7, 6, 5, 4, 3},
                                                {1},
                                                {11, 2, 1, 14, 15, 7, 8, 4, 5}};
  for (auto& test_entry : test_data_table) {
    RBTree<int, int> tree;
    for (auto key : test_entry) {
      static constexpr int VALUE = 1;
      tree.Insert({key, VALUE});
    }
    std::sort(test_entry.begin(), test_entry.end(), std::greater<>());
    auto tree_iterator = --tree.end();
    for (auto key : test_entry) {
      ASSERT_EQ(key,
                (tree_iterator--)->first);  // Checks if we get keys out in
                                            // reverse-sorted order out the
                                            // RBTree when iterating through it
    }
  }
}

int main() {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
