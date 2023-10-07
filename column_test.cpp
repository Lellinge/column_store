//
// Created by lars on 07.10.23.
//


#include "gtest/gtest.h"

#include "column.h"

TEST(COLUMN, BASIC_INSERT) {
    Column test_colum("score");
    EXPECT_NE(test_colum.write_value(5678), -1);
    EXPECT_NE(test_colum.write_value(347621), -1);
    EXPECT_NE(test_colum.write_value(-345), -1);
}

TEST(COLUMN, INSERT_AND_READ_BACK) {
    Column test_colum("score");
    auto row_id = test_colum.write_value(435);
    auto value = test_colum.read_value(row_id);
    EXPECT_EQ(value, 435);
    // make sure appending agains doesnt overwrite
    test_colum.write_value(799);
    value = test_colum.read_value(row_id);
    EXPECT_EQ(value, 435);
}

