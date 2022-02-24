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
  RBTree<int, int> tree;
  std::array<int, 8> test_keys{8, 5, 34, 3, 2, 99, 6, 7};
  for (auto key : test_keys) {
    static constexpr auto VALUE = 10;
    tree.Insert({key, VALUE});
  }
  std::sort(test_keys.begin(), test_keys.end());
  auto it = test_keys.begin();
  for (const auto& [key, value] : tree) {
    ASSERT_EQ(*(it++), key);
  }

  // Try a different sequence. Added this because the previous case was not
  // catching a particular insert sequence
  test_keys = {10, 8, 9, 7, 6, 5, 4, 3};
  RBTree<int, int> tree2;
  for (auto key : test_keys) {
    static constexpr auto VALUE = 10;
    tree2.Insert({key, VALUE});
  }
  std::sort(test_keys.begin(), test_keys.end());
  it = test_keys.begin();
  for (const auto& [key, value] : tree2) {
    ASSERT_EQ(*(it++), key);
  }
}

TEST(TEST_RB_TREE, TestReverseIteration) {
  RBTree<int, int> tree;
  std::array<int, 8> test_keys{8, 5, 34, 3, 2, 99, 6, 7};
  for (auto key : test_keys) {
    static constexpr auto VALUE = 10;
    tree.Insert({key, VALUE});
  }
  std::sort(test_keys.begin(), test_keys.end(), std::greater<>());
  auto it = --tree.end();
  for (auto test_key : test_keys) {
    ASSERT_EQ(test_key, (it--)->first);
  }

  // Try a different sequence. Added this because the previous case was not
  // catching a particular insert sequence
  test_keys = {10, 8, 9, 7, 6, 5, 4, 3};
  RBTree<int, int> tree2;
  for (auto key : test_keys) {
    static constexpr auto VALUE = 10;
    tree2.Insert({key, VALUE});
  }
  std::sort(test_keys.begin(), test_keys.end(), std::greater<>());
  it = --tree2.end();
  for (auto test_key : test_keys) {
    ASSERT_EQ(test_key, (it--)->first);
  }
}

int main() {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
