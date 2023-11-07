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

std::vector<DB_Value> Table::read_value(std::int64_t id) {
    std::vector<DB_Value> values;
    for (int i = 0; i < columns.size(); ++i) {
        DB_Value val;
        DB_val_content content{};
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

std::vector<std::int64_t> Table::bigger_than(int column_index, DB_Value value) {
    auto& column_to_search = columns.at(column_index);
    if (column_to_search.type != value.type) {
        std::cout << "type von DB_value passt nicht zum type von der column." << std::endl;
        return {};
    }
    switch (column_to_search.type) {

        case column::INT_64:
            return column::bigger_than(column_to_search, value.content.INT_64);
            break;
        case column::FLOAT_64:
            return column::bigger_than(column_to_search, value.content.FLOAT_64);
            break;
        case column::INT_32:
            return column::bigger_than(column_to_search, value.content.INT_32);
            break;
        case column::FLOAT_32:
            return column::bigger_than(column_to_search, value.content.FLOAT_32);
            break;
        case column::INT_8:
            return column::bigger_than(column_to_search, value.content.INT_8);
            break;
    }

}

std::int64_t Table::update_values(std::vector<DB_Value> values, std::int64_t id) {
    if (values.size() != columns.size()) {
        std::cout << "tried to pass in a number of values which doesnt match the number of columns." << std::endl;
        return -1;
    }
    for (int i = 0; i < values.size(); ++i) {
        switch (values.at(i).type) {

            case column::INT_64:
                column::update<std::int64_t>(this->columns.at(i), values.at(i).content.INT_64, id);
                break;
            case column::FLOAT_64:
                column::update<double>(this->columns.at(i), values.at(i).content.FLOAT_64, id);
                break;
            case column::INT_32:
                column::update<std::int32_t>(this->columns.at(i), values.at(i).content.INT_32, id);
                break;
            case column::FLOAT_32:
                column::update<float>(this->columns.at(i), values.at(i).content.FLOAT_32, id);
                break;
            case column::INT_8:
                column::update<std::int8_t>(this->columns.at(i), values.at(i).content.INT_8, id);
                break;
        }
    }
    return id;
}

std::int64_t Table::get_last_id() {
    return columns.at(0).get_last_id();
}

DB_Value::DB_Value(DB_val_content content, column::COLUMN_DATATYPES type) {
    this->content = content;
    this->type = type;
}

DB_Value::DB_Value() {
    this->content.INT_64 = 0;
    this->type = column::INT_64;
}
