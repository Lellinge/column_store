//
// Created by larse on 05.11.2023.
//

#include "operators.h"

#include <utility>

operators::Table::Table(::Table* source) {
    table = source;
    next_id = 0;
    last_id = source->get_last_id();
}

std::vector<DB_Value> operators::Table::pull() {
    if (this->next_id == -1) {
        return {};
    }
    auto val = this->table->read_value(next_id);
    if (next_id == last_id) {
        next_id = -1;
    } else {
        next_id = next_id + 1;
    }
    return val;
}

operators::Filter::Filter(std::shared_ptr<Operator> source, int column, operators::FilterType type, DB_Value value) {
    this->source = std::move(source);
    this->column = column;
    this->filterType = type;
    this->filterValue = value;
}

// TODO make generic
bool bigger_than_dbvalue(DB_Value bigger_than, DB_Value to_test) {
    if (bigger_than.type != to_test.type) {
        std::cout << "type of parameters to bigger_than_dbvalue doesnt match. pls fix." << std::endl;
        return false;
    }
    switch (bigger_than.type) {

        case column::INT_64:
            if (to_test.content.INT_64 > bigger_than.content.INT_64) {
                return true;
            } else {
                return false;
            }
            break;
        case column::FLOAT_64:
            if (to_test.content.FLOAT_64 > bigger_than.content.FLOAT_64) {
                return true;
            }  else {
                return false;
            }
            break;
        case column::INT_32:
            return (to_test.content.INT_32 > bigger_than.content.INT_32);
            break;
        case column::FLOAT_32:
            return (to_test.content.FLOAT_32 > bigger_than.content.FLOAT_32);
            break;
        case column::INT_8:
            return (to_test.content.INT_8 > bigger_than.content.INT_8);
            break;
    }
}
bool bigger_or_equal_dbvalue(DB_Value bigger_than, DB_Value to_test) {
    if (bigger_than.type != to_test.type) {
        std::cout << "type of parameters to bigger_than_dbvalue doesnt match. pls fix." << std::endl;
        return false;
    }
    switch (bigger_than.type) {

        case column::INT_64:
            if (to_test.content.INT_64 >= bigger_than.content.INT_64) {
                return true;
            } else {
                return false;
            }
            break;
        case column::FLOAT_64:
            if (to_test.content.FLOAT_64 >= bigger_than.content.FLOAT_64) {
                return true;
            }  else {
                return false;
            }
            break;
        case column::INT_32:
            return (to_test.content.INT_32 >= bigger_than.content.INT_32);
            break;
        case column::FLOAT_32:
            return (to_test.content.FLOAT_32 >= bigger_than.content.FLOAT_32);
            break;
        case column::INT_8:
            return (to_test.content.INT_8 >= bigger_than.content.INT_8);
            break;
    }
}

bool less_than_dbvalue(DB_Value less_than, DB_Value to_test) {
    if (less_than.type != to_test.type) {
        std::cout << "type of parameters to bigger_than_dbvalue doesnt match. pls fix." << std::endl;
        return false;
    }
    switch (less_than.type) {

        case column::INT_64:
            if (to_test.content.INT_64 < less_than.content.INT_64) {
                return true;
            } else {
                return false;
            }
            break;
        case column::FLOAT_64:
            if (to_test.content.FLOAT_64 < less_than.content.FLOAT_64) {
                return true;
            }  else {
                return false;
            }
            break;
        case column::INT_32:
            return (to_test.content.INT_32 < less_than.content.INT_32);
            break;
        case column::FLOAT_32:
            return (to_test.content.FLOAT_32 < less_than.content.FLOAT_32);
            break;
        case column::INT_8:
            return (to_test.content.INT_8 < less_than.content.INT_8);
            break;
    }
}



bool less_or_equal_dbvalue(DB_Value less_than, DB_Value to_test) {
    if (less_than.type != to_test.type) {
        std::cout << "type of parameters to bigger_than_dbvalue doesnt match. pls fix." << std::endl;
        return false;
    }
    switch (less_than.type) {

        case column::INT_64:
            if (to_test.content.INT_64 <= less_than.content.INT_64) {
                return true;
            } else {
                return false;
            }
            break;
        case column::FLOAT_64:
            if (to_test.content.FLOAT_64 <= less_than.content.FLOAT_64) {
                return true;
            }  else {
                return false;
            }
            break;
        case column::INT_32:
            return (to_test.content.INT_32 <= less_than.content.INT_32);
            break;
        case column::FLOAT_32:
            return (to_test.content.FLOAT_32 <= less_than.content.FLOAT_32);
            break;
        case column::INT_8:
            return (to_test.content.INT_8 <= less_than.content.INT_8);
            break;
    }
}

bool equal_dbvalue(DB_Value less_than, DB_Value to_test) {
    if (less_than.type != to_test.type) {
        std::cout << "type of parameters to bigger_than_dbvalue doesnt match. pls fix." << std::endl;
        return false;
    }
    switch (less_than.type) {

        case column::INT_64:
            if (to_test.content.INT_64 == less_than.content.INT_64) {
                return true;
            } else {
                return false;
            }
            break;
        case column::FLOAT_64:
            if (to_test.content.FLOAT_64 == less_than.content.FLOAT_64) {
                return true;
            }  else {
                return false;
            }
            break;
        case column::INT_32:
            return (to_test.content.INT_32 == less_than.content.INT_32);
            break;
        case column::FLOAT_32:
            return (to_test.content.FLOAT_32 == less_than.content.FLOAT_32);
            break;
        case column::INT_8:
            return (to_test.content.INT_8 == less_than.content.INT_8);
            break;
    }
}


std::vector<DB_Value> operators::Filter::pull() {
    // pull rows bis eine passt
    while (true) {
        auto row = source->pull();
        if (row.empty()) {
            return {};
        }
        bool passed_filter = false;
        switch (filterType) {

            case BIGGER_THAN:
                passed_filter = bigger_than_dbvalue(filterValue, row.at(column));
                break;
            case BIGGER_OR_EQUAL:
                passed_filter = bigger_or_equal_dbvalue(filterValue, row.at(column));
                break;
            case LESS_THAN:
                passed_filter = less_than_dbvalue(filterValue, row.at(column));
                break;
            case LESS_OR_EQUAL:
                passed_filter = less_or_equal_dbvalue(filterValue, row.at(column));
                break;
            case EQUAL:
                passed_filter = equal_dbvalue(filterValue, row.at(column));
                break;
        }
        if (passed_filter) {
            return row;
        }
    }
}
