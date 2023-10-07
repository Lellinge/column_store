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

void Table::read_all() {
    int64_t last_id_1 = columns.first.get_last_id();
    int64_t last_id_2 = columns.second.get_last_id();
    if (last_id_1 != last_id_2) {
        std::cout << "column 1 and 2 have a different number of rows. Something went wrong." << std::endl;
        return;
    }

    std::cout << "id\t\tvalue_1\t\tvalue_2" << std::endl;
    for (int i = 0; i < last_id_1; ++i) {
        auto value_1 = columns.first.read_value(i);
        auto value_2 = columns.second.read_value(i);

        std::cout << i;
        std::cout << "\t\t" << value_1;
        std::cout << "\t\t" << value_2;
        std::cout << std::endl;
    }
}

std::pair<std::int64_t, double> Table::read_value(std::int64_t id) {
    auto value_1 = columns.first.read_value(id);
    auto value_2 = columns.second.read_value(id);
    return std::pair<std::int64_t ,double>{value_1, value_2};
}
