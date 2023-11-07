//
// Created by larse on 05.11.2023.
//

#include "operators.h"

int main() {

    Table table("testtable", std::vector<column::COLUMN_DATATYPES>{column::INT_64, column::FLOAT_64});
    std::shared_ptr<operators::Table> source_ptr = std::make_shared<operators::Table>(&table);
    auto value = create_db_value<std::int64_t>(0);
    operators::Filter filter(source_ptr, 0, operators::BIGGER_THAN, value);
    int number_rows = 0;
    while (true) {
        auto row = filter.pull();
        if (row.empty()) {
            break;
        }
        number_rows++;
        std::cout << row.size() << "  " << row.at(0).content.INT_64 << std::endl;
    }
    std::cout << "got " << number_rows << " rows" << std::endl;

}