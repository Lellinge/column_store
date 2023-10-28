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
#include <vector>

namespace column {

    template<typename T>
    class Tuple {
    public:
        Tuple(T value) {
            this->value = value;
        }
        T value;
    };

    // TODO evtl mehr, mal schauen
    enum COLUMN_DATATYPES {
        INT_64,
        FLOAT_64,
        INT_32,
        FLOAT_32,
        INT_8
    };
    
    std::string datatype_to_str(COLUMN_DATATYPES type);


    class Column {
    public:
        FILE *fd;
        std::string name;

        std::int64_t last_id;

        int fd_int;
        void *mapped;

        COLUMN_DATATYPES type;


        Column(const std::string &name, COLUMN_DATATYPES type) {
            this->type = type;

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

            this->mapped = mmap(nullptr, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_int, 0);
            if (mapped == MAP_FAILED) {
                std::cout << std::strerror(errno) << std::endl;
                std::cout << "failed to mmap " << std::endl;
                return;
            }
            auto length_tuple = 0;
            switch (type) {

                case INT_64:
                    length_tuple += sizeof(std::int64_t);
                    break;
                case FLOAT_64:
                    length_tuple += sizeof(double);
                    break;
                case INT_32:
                    length_tuple += sizeof(std::int32_t);
                    break;
                case FLOAT_32:
                    length_tuple += sizeof(float);
                    break;
                case INT_8:
                    length_tuple += sizeof(char);
                    break;
                default:
                    std::cout << "unknown datatype, please implement length at:" << __LINE__ << std::endl;
            }
            int length = statbuf.st_size / length_tuple;
            // ids start at 0
            this->last_id = length - 1;
        }


        [[nodiscard]] std::int64_t get_last_id() const {
            return this->last_id;
        }

        void msync() {
            auto size_tuple = 0;
            switch (type) {

                case INT_64:
                    size_tuple += sizeof(std::int64_t);
                    break;
                case FLOAT_64:
                    size_tuple += sizeof(double);
                    break;
                case INT_32:
                    size_tuple += sizeof(std::int32_t);
                    break;
                case FLOAT_32:
                    size_tuple += sizeof(float);
                    break;
                case INT_8:
                    size_tuple += sizeof(std::int8_t);
                    break;
            }
            ::msync(mapped, size_tuple * (last_id + 1), MS_SYNC);
        }
    };

    template<typename T>
    bool write(Column& column1, Tuple<T> tuple) {
        auto size_tuple = sizeof(T);
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
    std::int64_t write_value(Column& column1,T value) {
        Tuple new_tuple(value);
        bool worked = write(column1, new_tuple);
        if (worked) {
            return column1.last_id;
        } else {
            return -1;
        }
    }

    // TODO make it handle invalid ids
    template<typename T>
    T read_value(Column& column1, std::int64_t id) {
        return *value_at_id<T>(column1, id);
    }

    template<typename T>
    T *value_at_id(Column& column1, std::int64_t id) {
        auto *mapped_cast = (std::int8_t *) column1.mapped;
        auto length_tuple = sizeof(T);
        auto offset = id * length_tuple;
        std::int8_t *address_of_id = mapped_cast + offset;
        T* address_of_value = (T *) address_of_id;
        T *value = address_of_value;
        return value;
    }



    template<typename T>
    void read_all(Column& column1) {
        // TODO make work with templates
        std::cout << "id\tvalue" << std::endl;
        auto length_tuple = sizeof(T) + sizeof(std::int64_t);
        for (int i = 0; i <= column1.last_id; ++i) {
            std::cout << i << "\t";
            std::cout << value_at_id<T>(column1, i) << std::endl;
        }
    }

    template<typename T>
    std::vector<std::int64_t> bigger_than(Column& column1, T value) {
        std::vector<std::int64_t> ret_vect;
        for (int i = 0; i <= column1.get_last_id(); ++i) {
            auto val = value_at_id<T>(column1, i);
            if (*val > value) {
                ret_vect.push_back(i);
            }
        }
        return ret_vect;
    }

    template<typename T>
    bool update(Column& column1, T value, std::int64_t id) {
        auto *mapped_cast = (std::int8_t*) column1.mapped;
        auto length_tuple = sizeof(T);
        auto offset = id * length_tuple;
        T *address_of_value;
        address_of_value = (T*) (mapped_cast + offset);

        *address_of_value = value;

        column1.msync();
        return true;
    }

}

#endif //COLUMN_STORE_COLUMN_H

