//
// Created by kevin on 2/22/22.
//
#include <gtest/gtest.h>

#include "map.h"

TEST(TEST_MAP, TestEmptyOnConstruction)
{
    Map<int, int> tree;
    ASSERT_EQ(0, tree.Size());
    ASSERT_EQ(true, tree.Empty());
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
