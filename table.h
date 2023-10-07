//
// Created by lars on 07.10.23.
//

#ifndef COLUMN_STORE_TABLE_H
#define COLUMN_STORE_TABLE_H


#include "column.h"



class Table {
    std::pair<Column<std::int64_t>, Column<double>> columns;
public:
    Table();

    std::int64_t insert_values(std::int64_t value_1, double value_2);

    void read_all();

    std::pair<std::int64_t, double> read_value(std::int64_t id);
};


#endif //COLUMN_STORE_TABLE_H
