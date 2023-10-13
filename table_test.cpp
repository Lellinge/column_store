//
// Created by lars on 07.10.23.
//

#include "table.h"

#include "gtest/gtest.h"

TEST(TABLE, BASIC_INSERT) {
    Table table("testtable", std::vector<column::COLUMN_DATATYPES>{column::INT_64, column::FLOAT_64});
    auto value_1 = create_db_value<std::int64_t>(1);
    auto value_2 = create_db_value<double>(2.5);
    auto value_3 = create_db_value<std::int64_t>(6587);
    auto value_4 = create_db_value<double>(345.2345);
    auto value_5 = create_db_value<std::int64_t>(-2143);
    auto value_6 = create_db_value<double>(-3452.5);
    EXPECT_NE(table.insert_values(std::vector<DB_Value>{value_1, value_2}), -1);
    EXPECT_NE(table.insert_values(std::vector<DB_Value>{value_3, value_4}), -1);
    EXPECT_NE(table.insert_values(std::vector<DB_Value>{value_4, value_5}), -1);
}

TEST(TABLE, READ_ALL) {
    Table table("testtable", std::vector<column::COLUMN_DATATYPES>{column::INT_64, column::FLOAT_64});
    table.read_all();
}

TEST(TABLE, INSERT_AND_READ_BACK)  {
    Table table("testtable", std::vector<column::COLUMN_DATATYPES>{column::INT_64, column::FLOAT_64});
    DB_Value val_1 = create_db_value<std::int64_t>(234);
    DB_Value val_2 = create_db_value<double>(345.87);
    auto row_id = table.insert_values(std::vector<DB_Value>{val_1, val_2});
    auto value = table.read_value(row_id);
    EXPECT_EQ(value[0].content.INT_64, 234);
    EXPECT_NEAR(value[1].content.FLOAT_64, 345.87, 0.1);

    // sicherstellen, das weiteres einfügen nicht überschreibt.
    DB_Value new_1 = create_db_value<std::int64_t>(3456);
    DB_Value new_2 = create_db_value<double>(3245.123);
    table.insert_values(std::vector<DB_Value>{new_1, new_2});
    auto value_2 = table.read_value(row_id);
    EXPECT_EQ(value_2[0].content.INT_64, 234);
    EXPECT_NEAR(value_2[1].content.FLOAT_64, 345.87, 0.1);
}

