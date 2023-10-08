//
// Created by lars on 07.10.23.
//

#ifndef COLUMN_STORE_TABLE_H
#define COLUMN_STORE_TABLE_H

#include <vector>

#include "column.h"



class Table {
public:
    std::string name;
    std::vector<column::Column> columns;


    Table(std::string name, std::vector<column::COLUMN_DATATYPES> columns);

    std::int64_t insert_values(std::int64_t value_1, double value_2);

    void read_all();

    std::pair<std::int64_t, double> read_value(std::int64_t id);
};


#endif //COLUMN_STORE_TABLE_H
