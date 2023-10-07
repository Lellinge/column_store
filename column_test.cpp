//
// Created by lars on 07.10.23.
//


#include "gtest/gtest.h"

#include "column.h"

TEST(COLUMN, BASIC_INSERT) {
    Column test_colum("score");
    EXPECT_EQ(test_colum.write_value(55), true);
    EXPECT_EQ(test_colum.write_value(345634), true);
    EXPECT_EQ(test_colum.write_value(-2452345), true);
}