//
// Created by lars on 07.10.23.
//

#ifndef COLUMN_STORE_TABLE_H
#define COLUMN_STORE_TABLE_H

#include <vector>
#include <exception>

#include "column.h"

union DB_val_content {
    float FLOAT_32;
    double FLOAT_64;
    std::int64_t INT_64;
    std::int32_t INT_32;
    std::int8_t INT_8;
};

class DB_Value {
public:
    DB_val_content content{};
    column::COLUMN_DATATYPES type;
    DB_Value(DB_val_content content, column::COLUMN_DATATYPES type);
    DB_Value();
};


class Table {
public:
    std::string name;
    std::vector<column::Column> columns;


    Table(const std::string& name, const std::vector<column::COLUMN_DATATYPES>& columns);

    std::int64_t insert_values(std::vector<DB_Value> values);

    std::int64_t update_values(std::vector<DB_Value> values, std::int64_t id);

    void read_all();

    std::vector<DB_Value> read_value(std::int64_t id);

    std::vector<std::int64_t> bigger_than(int column, DB_Value value);
};


template<typename T>
DB_Value create_db_value(T value) {
    bool allowed = false;
    DB_val_content content{};
    if (std::is_same<T, std::int64_t>::value) {
        allowed = true;
        content.INT_64 = value;
        return DB_Value(content, column::INT_64);
    } else if (std::is_same<T, std::int32_t>::value) {
        allowed = true;
        content.INT_32 = value;
        return DB_Value(content, column::INT_32);
    } else if (std::is_same<T, std::int8_t>::value) {
        allowed = true;
        content.INT_8 = value;
        return DB_Value(content, column::INT_8);
    } else if (std::is_same<T, double>::value) {
        allowed = true;
        content.FLOAT_64 = value;
        return DB_Value(content, column::FLOAT_64);
    } else if (std::is_same<T, float>::value) {
        allowed = true;
        content.FLOAT_32 = value;
        return DB_Value(content, column::FLOAT_32);
    } else {
        std::cout << "Unknown type used for creation of db value, returning null pointer." << std::endl;
        throw new std::invalid_argument("Unknown type used for instation of create_db_value. Please fix.");
    }
}


#endif //COLUMN_STORE_TABLE_H
