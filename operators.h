//
// Created by larse on 05.11.2023.
//

#ifndef COLUMN_STORE_OPERATORS_H
#define COLUMN_STORE_OPERATORS_H


#include "table.h"

#include <coro/generator.hpp>
#include <memory>


namespace operators {
    class Operator {
    public:
        virtual std::vector<DB_Value> pull() = 0;
    };
    class Table : public Operator{
    public:
        // -1 => alle Reihen wurden geholt
        std::int64_t next_id;
        std::int64_t last_id;
        ::Table* table;
        explicit Table(::Table* table);
        std::vector<DB_Value> pull() override;
    };


    enum FilterType {
        BIGGER_THAN,
        BIGGER_OR_EQUAL,
        LESS_THAN,
        LESS_OR_EQUAL,
        EQUAL,
    };

    class Filter : public Operator{
    public:
        int column;
        FilterType filterType;
        DB_Value filterValue;
        std::shared_ptr<Operator> source;
        Filter(std::shared_ptr<Operator> source, int column, FilterType type, DB_Value value);
        std::vector<DB_Value> pull() override;
    };
}



#endif //COLUMN_STORE_OPERATORS_H
