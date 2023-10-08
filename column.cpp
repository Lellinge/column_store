//
// Created by lars on 07.10.23.
//

#include "column.h"

std::string column::datatype_to_str(column::COLUMN_DATATYPES type) {
    switch (type) {

        case INT_64:
            return "INT_64";
            break;
        case FLOAT_64:
            return "FLOAT_64";
            break;
        case INT_32:
            return "INT_32";
            break;
        case FLOAT_32:
            return "FLOAT_32";
            break;
        case INT_8:
            return "INT_8";
            break;
    }
}
