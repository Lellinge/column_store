//
// Created by lars on 07.10.23.
//

#include "table.h"

Table::Table(): columns(Column<std::int64_t>("table_int64"), Column<double>("table_double")) {
}

std::int64_t Table::insert_values(std::int64_t value_1, double value_2) {
    std::int64_t id_1 = columns.first.write_value(value_1);
    std::int64_t id_2 = columns.second.write_value(value_2);
    if (id_1 == id_2) {
        return id_1;
    } else {
        std::cout << "wtf, ids unterscheiden sich." << std::endl;
        return -1;
    }
}
