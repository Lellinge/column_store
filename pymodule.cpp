//
// Created by larse on 14.10.2023.
//

#include <pybind11/pybind11.h>

#include "table.h"

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(pystore, mod) {
    mod.doc() = "testing pybind11";

    mod.def("add", &add, "adds to numbers");


    py::class_<DB_Value>(mod, "db_value")
            .def(py::init<DB_val_content, column::COLUMN_DATATYPES>());

    py::enum_<column::COLUMN_DATATYPES>(mod, "column_datatypes")
            .value("INT_64", column::COLUMN_DATATYPES::INT_64)
            .value("INT_32", column::COLUMN_DATATYPES::INT_32)
            .value("INT_8", column::COLUMN_DATATYPES::INT_8)
            .value("FLOAT_64", column::COLUMN_DATATYPES::FLOAT_64)
            .value("FLOAT_32", column::COLUMN_DATATYPES::FLOAT_32);


    py::class_<DB_val_content>(mod, "db_val_content")
            .def(py::init<>())
            .def_property("FLOAT_32",
                          [](DB_val_content& self) -> const float& {
                return self.FLOAT_32;
            },
                          [](DB_val_content& self, const float& value) {
                self.FLOAT_32 = value;
            })
            .def_property("FLOAT_64",
                          [](DB_val_content& self) -> const double& {
                return self.FLOAT_64;
            }, [](DB_val_content& self, const double& value) {
                self.FLOAT_64 = value;
            })
            .def_property("INT_64",
                          [](DB_val_content& self) -> const std::int64_t& {
                return self.INT_64;
            },  [](DB_val_content& self, const std::int64_t& value) {
                self.INT_64 = value;
            })
            .def_property("INT_32",
                           [](DB_val_content& self) -> const std::int32_t& {
                return self.INT_32;
            }, [](DB_val_content& self, const std::int32_t& value) {
                self.INT_32 = value;
            })
            .def_property("INT_8",
                          [](DB_val_content& self) -> const std::int8_t& {
                return self.INT_8;
            }, [](DB_val_content& self, const std::int8_t& value) {
                self.INT_8 = value;
            });

}
