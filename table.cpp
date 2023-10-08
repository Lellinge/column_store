//
// Created by lars on 07.10.23.
//

#include "table.h"
#include <map>


std::int64_t Table::insert_values(std::int64_t value_1, double value_2) {
    std::int64_t id_1 = column::write_value<std::int64_t>(columns[0], value_1);
    std::int64_t id_2 = column::write_value<double>(columns[1], value_2);
    if (id_1 == id_2) {
        return id_1;
    } else {
        std::cout << "wtf, ids unterscheiden sich." << std::endl;
        return -1;
    }
}

void Table::read_all() {
    int64_t last_id_1 = columns[0].get_last_id();
    int64_t last_id_2 = columns[1].get_last_id();
    if (last_id_1 != last_id_2) {
        std::cout << "column 1 and 2 have a different number of rows. Something went wrong." << std::endl;
        return;
    }

    std::cout << "id\t\tvalue_1\t\tvalue_2" << std::endl;
    for (int i = 0; i < last_id_1; ++i) {
        auto value_1 = column::read_value<std::int64_t>(columns[0], i);
        double value_2 = column::read_value<double>(columns[1], i);

        std::cout << i;
        std::cout << "\t\t" << value_1;
        std::cout << "\t\t" << value_2;
        std::cout << std::endl;
    }
}

std::pair<std::int64_t, double> Table::read_value(std::int64_t id) {
    auto value_1 = column::read_value<std::int64_t>(columns[0], id);
    auto value_2 = column::read_value<double>(columns[1], id);
    return std::pair<std::int64_t ,double>{value_1, value_2};
}

Table::Table(std::string name, std::vector<column::COLUMN_DATATYPES> columns_new) {
    this->name = name;
    std::map<column::COLUMN_DATATYPES, int> occurances;
    for (auto&& column_ind : columns_new) {
        auto found = occurances.find(column_ind);
        int id;
        if (found == occurances.end()) {
            id = 0;
            occurances[column_ind] = 1;
        } else {
            id = found->second;
            occurances[column_ind] = id + 1;
        }
        std::string name_column = name + "_" + column::datatype_to_str(column_ind) + "_" + std::to_string(id);
        std::cout << "name of column: " << name_column << std::endl;
        this->columns.emplace_back(name_column,column_ind);
    }
}

