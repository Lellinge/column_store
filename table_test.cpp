//
// Created by lars on 07.10.23.
//

#include "table.h"

#include "gtest/gtest.h"

TEST(TABLE, BASIC_INSERT) {
    Table table;
    EXPECT_NE(table.insert_values(1, 2.5), -1);
    EXPECT_NE(table.insert_values(6587, 345.2345), -1);
    EXPECT_NE(table.insert_values(-2143, -3452.5), -1);
}
