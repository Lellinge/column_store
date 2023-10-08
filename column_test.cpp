//
// Created by lars on 07.10.23.
//


#include "gtest/gtest.h"

#include "column.h"

#include <filesystem>


TEST(COLUMN, BASIC_INSERT) {
    column::Column test_colum("score", column::INT_64);
    EXPECT_NE(column::write_value<std::int64_t>(test_colum,5678), -1);
    EXPECT_NE(column::write_value<std::int64_t>(test_colum,-34578), -1);
    EXPECT_NE(column::write_value<std::int64_t>(test_colum,23658), -1);
}

TEST(COLUMN, INSERT_AND_READ_BACK) {
    column::Column test_colum("score", column::INT_64);
    auto row_id = column::write_value<std::int64_t>(test_colum, 435);
    auto value = column::read_value<std::int64_t>(test_colum, row_id);
    EXPECT_EQ(value, 435);
    // make sure appending agains doesnt overwrite
    column::write_value<std::int64_t>(test_colum, 799);
    value = column::read_value<std::int64_t>(test_colum, row_id);
    EXPECT_EQ(value, 435);
}

TEST(COLUMN, DOUBLE_BASIC_INSERT) {
    column::Column test_colum("double", column::FLOAT_64);
    EXPECT_NE(column::write_value<double>(test_colum, 5678.3456), -1);
    EXPECT_NE(column::write_value<double>(test_colum, 1948.39266), -1);
    EXPECT_NE(column::write_value<double>(test_colum, 4.3246), -1);
}

TEST(COLUMN, DOUBLE_INSERT_AND_READ_BACK) {
    column::Column test_colum("double", column::FLOAT_64);
    auto row_id = column::write_value<double>(test_colum, 435.3456);
    auto value = column::read_value<double>(test_colum, row_id);
    EXPECT_EQ(value, 435.3456);
    // make sure appending agains doesnt overwrite
    column::write_value<double>(test_colum, 799.01982);
    value = column::read_value<double>(test_colum, row_id);
    EXPECT_EQ(value, 435.3456);
}

TEST(COLUMN, FLOAT_BASIC_INSERT) {
    column::Column test_colum("float", column::FLOAT_32);
    EXPECT_NE(column::write_value<float>(test_colum, 5678.3456), -1);
    EXPECT_NE(column::write_value<float>(test_colum, 1948.39266), -1);
    EXPECT_NE(column::write_value<float>(test_colum, 4.3246), -1);
}

TEST(COLUMN, FLOAT_INSERT_AND_READ_BACK) {
    column::Column test_colum("float", column::FLOAT_32);
    auto row_id = column::write_value<float>(test_colum, 435.346);
    auto value = column::read_value<float>(test_colum, row_id);
    EXPECT_NEAR(value, 435.346, 0.1);
    // make sure appending agains doesnt overwrite
    auto row_id_2 = column::write_value<float>(test_colum, 799.01982);
    value = column::read_value<float>(test_colum, row_id);
    EXPECT_NEAR(value, 435.346, 0.1);
    value = column::read_value<float>(test_colum, row_id_2);
    EXPECT_NEAR(value, 799.01982, 0.1);
    column::read_all<float>(test_colum);
}

TEST(COLUMN, INT_BASIC_INSERT) {
    column::Column test_colum("int", column::INT_32);
    EXPECT_NE(column::write_value<int>(test_colum, 23456), -1);
    EXPECT_NE(column::write_value<int>(test_colum, 2058), -1);
    EXPECT_NE(column::write_value<int>(test_colum, -346), -1);
}

TEST(COLUMN, INT_INSERT_AND_READ_BACK) {
    column::Column test_colum("int", column::INT_32);
    auto row_id = column::write_value<int>(test_colum, 435);
    auto value = column::read_value<int>(test_colum, row_id);
    EXPECT_EQ(value, 435);
    // make sure appending agains doesnt overwrite
    column::write_value<int>(test_colum, 799);
    value = column::read_value<int>(test_colum, row_id);
    EXPECT_EQ(value, 435);
}

TEST(COLUMN, CHAR_BASIC_INSERT) {
    column::Column test_colum("char", column::INT_8);
    EXPECT_NE(column::write_value<char>(test_colum, 75), -1);
    EXPECT_NE(column::write_value<char>(test_colum, 7), -1);
    EXPECT_NE(column::write_value<char>(test_colum, 23), -1);
    column::read_all<char>(test_colum);
}

TEST(COLUMN, CHAR_READ_ALL) {
    column::Column test_column("char", column::INT_8);
    column::read_all<char>(test_column);
}

TEST(COLUMN, CHAR_INSERT_AND_READ_BACK) {
    column::Column test_colum("char", column::INT_8);
    auto row_id = column::write_value<char>(test_colum, 43);
    std::cout << row_id << std::endl;
    auto value = column::read_value<char>(test_colum, row_id);
    EXPECT_EQ(value, 43);
    // make sure appending agains doesnt overwrite
    column::write_value<char>(test_colum, 99);
    value = column::read_value<char>(test_colum, row_id);
    EXPECT_EQ(value, 43);
}

