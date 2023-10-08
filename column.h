//
// Created by lars on 07.10.23.
//

#ifndef COLUMN_STORE_COLUMN_H
#define COLUMN_STORE_COLUMN_H


#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace column {

    template<typename T>
    class Tuple {
    public:
        Tuple(std::int64_t id, T value) {
            this->id = id;
            this->value = value;
        }

        std::int64_t id;
        T value;
    };


    template<typename T>
    class Column {
    public:
        FILE *fd;
        std::string name;

        std::int64_t last_id;

        int fd_int;
        void *mapped;


        Column(const std::string &name) {
            this->name = name;
            // -1 = not yet calculated
            this->last_id = -1;


            this->fd_int = open(("../store/" + name).c_str(), O_RDWR | O_APPEND);
            struct stat statbuf;
            int error = fstat(this->fd_int, &statbuf);
            if (error == -1) {
                std::cout << strerror(errno) << std::endl;
                std::cout << "fstat failed." << std::endl;
            }

            this->mapped = mmap(nullptr, statbuf.st_size, PROT_READ, MAP_SHARED, fd_int, 0);
            if (mapped == MAP_FAILED) {
                std::cout << std::strerror(errno) << std::endl;
                std::cout << "failed to mmap " << std::endl;
                return;
            }
            auto length_tuple = sizeof(std::int64_t) + sizeof(T);
            int length = statbuf.st_size / length_tuple;
            // ids start at 0
            this->last_id = length - 1;
        }


        std::int64_t get_last_id() {
            return this->last_id;
        }
    };

    template<typename T>
    bool write(Column<T>& column1, Tuple<T> tuple) {
        auto size_tuple = sizeof(std::int64_t) + sizeof(T);
        auto write_ret = ::write(column1.fd_int, &tuple, size_tuple);
        // write gibt die anzahl an geschriebenen bytes zurück. es sollten immer 16 sein.
        if (write_ret != size_tuple) {
            std::cout << "error with writing." << std::endl;
            return false;
        }
        column1.last_id = column1.last_id + 1;
        auto ret_fsync = fsync(column1.fd_int);
        if (ret_fsync != 0) {
            std::cout << std::strerror(errno) << std::endl;
            std::cout << "fsync failed." << std::endl;
            return false;
        }
        return true;
    }

    template<typename T>
    std::int64_t write_value(Column<T>& column1,T value) {
        Tuple new_tuple(column1.last_id + 1, value);
        bool worked = write(column1, new_tuple);
        if (worked) {
            return column1.last_id;
        } else {
            return -1;
        }
    }

    // TODO make it handle invalid ids
    template<typename T>
    T read_value(Column<T>& column1, std::int64_t id) {
        return *value_at_id(column1, id);
    }

    template<typename T>
    T *value_at_id(Column<T>& column1, std::int64_t id) {
        auto *mapped_cast = (std::int8_t *) column1.mapped;
        auto length_tuple = sizeof(std::int64_t) + sizeof(T);
        auto offset = id * length_tuple;
        std::int8_t *address_of_id = mapped_cast + offset;
        auto addres_of_id_cast = (std::int64_t *) address_of_id;
        auto address_of_value = addres_of_id_cast + 1;
        T *value = ((T *) address_of_value);
        return value;
    }


    template<typename T>
    void read_all(Column<T>& column1) {
        // TODO make work with templates
        std::cout << "id\tvalue" << std::endl;
        auto length_tuple = sizeof(T) + sizeof(std::int64_t);
        for (int i = 0; i <= column1.last_id; ++i) {
            std::cout << i << "\t";
            std::cout << value_at_id(column1, i) << std::endl;
        }
    }


}

#endif //COLUMN_STORE_COLUMN_H

