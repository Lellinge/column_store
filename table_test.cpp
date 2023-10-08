//
// Created by lars on 07.10.23.
//

#include "table.h"

#include "gtest/gtest.h"

TEST(TABLE, BASIC_INSERT) {
    Table table("testtable", std::vector<column::COLUMN_DATATYPES>{column::INT_64, column::FLOAT_64});
    EXPECT_NE(table.insert_values(1, 2.5), -1);
    EXPECT_NE(table.insert_values(6587, 345.2345), -1);
    EXPECT_NE(table.insert_values(-2143, -3452.5), -1);
}

TEST(TABLE, READ_ALL) {
    Table table("testtable", std::vector<column::COLUMN_DATATYPES>{column::INT_64, column::FLOAT_64});
    table.read_all();
}

TEST(TABLE, INSERT_AND_READ_BACK)  {
    Table table("testtable", std::vector<column::COLUMN_DATATYPES>{column::INT_64, column::FLOAT_64});
    auto row_id = table.insert_values(234, 345.87);
    auto value = table.read_value(row_id);
    EXPECT_EQ(value.first, 234);
    EXPECT_NEAR(value.second, 345.87, 0.1);

    // sicherstellen, das weiteres einfügen nicht überschreibt.
    table.insert_values(3456, 3245.123);
    auto value_2 = table.read_value(row_id);
    EXPECT_EQ(value_2.first, 234);
    EXPECT_NEAR(value_2.second, 345.87, 0.1);
}
