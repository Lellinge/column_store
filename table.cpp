//
// Created by lars on 07.10.23.
//

#include "table.h"
#include <map>


std::int64_t Table::insert_values(std::vector<DB_Value> values) {
    if (values.size() != columns.size()) {
        std::cout << "tried to pass in a number of values which doesnt match the number of columns." << std::endl;
        return -1;
    }
    // TODO check if sizes mathc
    for (int i = 0; i < values.size(); ++i)  {
        switch (values.at(i).type) {
            case column::INT_64:
                column::write_value(this->columns.at(i), values.at(i).content.INT_64);
                break;
            case column::FLOAT_64:
                column::write_value(this->columns.at(i), values.at(i).content.FLOAT_64);
                break;
            case column::INT_32:
                column::write_value(this->columns.at(i), values.at(i).content.INT_32);
                break;
            case column::FLOAT_32:
                column::write_value(this->columns.at(i), values.at(i).content.FLOAT_32);
                break;
            case column::INT_8:
                column::write_value(this->columns.at(i), values.at(i).content.INT_8);
                break;
        }
    }
    return columns.at(0).get_last_id();
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
        auto value_2 = column::read_value<double>(columns[1], i);

        std::cout << i;
        std::cout << "\t\t" << value_1;
        std::cout << "\t\t" << value_2;
        std::cout << std::endl;
    }
}

DB_Value read_value_column_as_DB_Val(column::Column& column1, std::int64_t id) {
    switch (column1.type) {

        case column::INT_64:

            break;
        case column::FLOAT_64:
            break;
        case column::INT_32:
            break;
        case column::FLOAT_32:
            break;
        case column::INT_8:
            break;
    }
}

std::vector<DB_Value> Table::read_value(std::int64_t id) {
    std::vector<DB_Value> values;
    for (int i = 0; i < columns.size(); ++i) {
        DB_Value val;
        DB_val_content content;
        this->columns.at(i);
        switch (columns.at(i).type) {
            case column::INT_64:
                content.INT_64  = column::read_value<std::int64_t>(this->columns.at(i), id);
                val = create_db_value<std::int64_t>(content.INT_64);
                break;
            case column::FLOAT_64:
                content.FLOAT_64 = column::read_value<double>(this->columns.at(i), id);
                val = create_db_value<double>(content.FLOAT_64);
                break;
            case column::INT_32:
                content.INT_32 = column::read_value<int32_t>(this->columns.at(i), id);
                val = create_db_value<int32_t>(content.INT_32);
                break;
            case column::FLOAT_32:
                content.FLOAT_32 = column::read_value<float>(this->columns.at(i), id);
                val = create_db_value<float>(content.FLOAT_32);
                break;
            case column::INT_8:
                content.INT_8 = column::read_value<int8_t>(this->columns.at(i), id);
                break;
        }
        values.push_back(val);
    }
    return values;
}

Table::Table(const std::string& name, const std::vector<column::COLUMN_DATATYPES>& columns_new) {
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

DB_Value::DB_Value(DB_val_content content, column::COLUMN_DATATYPES type) {
    this->content = content;
    this->type = type;
}

DB_Value::DB_Value() {
    this->content.INT_64 = 0;
    this->type = column::INT_64;
}
