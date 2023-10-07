//
// Created by lars on 07.10.23.
//


#include "gtest/gtest.h"

#include "column.h"

#include <filesystem>


TEST(COLUMN, BASIC_INSERT) {
    Column<std::int64_t> test_colum("score");
    EXPECT_NE(test_colum.write_value(5678), -1);
    EXPECT_NE(test_colum.write_value(347621), -1);
    EXPECT_NE(test_colum.write_value(-345), -1);
}

TEST(COLUMN, INSERT_AND_READ_BACK) {
    Column<std::int64_t> test_colum("score");
    auto row_id = test_colum.write_value(435);
    auto value = test_colum.read_value(row_id);
    EXPECT_EQ(value, 435);
    // make sure appending agains doesnt overwrite
    test_colum.write_value(799);
    value = test_colum.read_value(row_id);
    EXPECT_EQ(value, 435);
}

TEST(COLUMN, DOUBLE_BASIC_INSERT) {
    Column<double> test_colum("double");
    EXPECT_NE(test_colum.write_value(5678.3456), -1);
    EXPECT_NE(test_colum.write_value(1948.39266), -1);
    EXPECT_NE(test_colum.write_value(4.3246), -1);
}

TEST(COLUMN, DOUBLE_INSERT_AND_READ_BACK) {
    Column<double> test_colum("double");
    auto row_id = test_colum.write_value(435.3456);
    auto value = test_colum.read_value(row_id);
    EXPECT_EQ(value, 435.3456);
    // make sure appending agains doesnt overwrite
    test_colum.write_value(799.01982);
    value = test_colum.read_value(row_id);
    EXPECT_EQ(value, 435.3456);
}

TEST(COLUMN, FLOAT_BASIC_INSERT) {
    Column<float> test_colum("float");
    EXPECT_NE(test_colum.write_value(5678.3456), -1);
    EXPECT_NE(test_colum.write_value(1948.39266), -1);
    EXPECT_NE(test_colum.write_value(4.3246), -1);
}

TEST(COLUMN, FLOAT_INSERT_AND_READ_BACK) {
    Column<float> test_colum("float");
    auto row_id = test_colum.write_value(435.346);
    auto value = test_colum.read_value(row_id);
    EXPECT_NEAR(value, 435.346, 0.1);
    // make sure appending agains doesnt overwrite
    auto row_id_2 = test_colum.write_value(799.01982);
    value = test_colum.read_value(row_id);
    EXPECT_NEAR(value, 435.346, 0.1);
    value = test_colum.read_value(row_id_2);
    EXPECT_NEAR(value, 799.01982, 0.1);
    test_colum.read_all();
}

TEST(COLUMN, INT_BASIC_INSERT) {
    Column<int> test_colum("int");
    EXPECT_NE(test_colum.write_value(23456), -1);
    EXPECT_NE(test_colum.write_value(2058), -1);
    EXPECT_NE(test_colum.write_value(-346), -1);
}

TEST(COLUMN, INT_INSERT_AND_READ_BACK) {
    Column<int> test_colum("int");
    auto row_id = test_colum.write_value(435);
    auto value = test_colum.read_value(row_id);
    EXPECT_EQ(value, 435);
    // make sure appending agains doesnt overwrite
    test_colum.write_value(799);
    value = test_colum.read_value(row_id);
    EXPECT_EQ(value, 435);
}

TEST(COLUMN, CHAR_BASIC_INSERT) {
    Column<char> test_colum("char");
    EXPECT_NE(test_colum.write_value(75), -1);
    EXPECT_NE(test_colum.write_value(7), -1);
    EXPECT_NE(test_colum.write_value(23), -1);
    test_colum.read_all();
}

TEST(COLUMN, CHAR_READ_ALL) {
    Column<char> test_column("char");
    test_column.read_all();
}

TEST(COLUMN, CHAR_INSERT_AND_READ_BACK) {
    Column<char> test_colum("char");
    auto row_id = test_colum.write_value(43);
    std::cout << row_id << std::endl;
    auto value = test_colum.read_value(row_id);
    EXPECT_EQ(value, 43);
    // make sure appending agains doesnt overwrite
    test_colum.write_value(99);
    value = test_colum.read_value(row_id);
    EXPECT_EQ(value, 43);
}

