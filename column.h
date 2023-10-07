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

template <typename T>
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
class Column  {
    FILE* fd;
    std::string name;

    std::int64_t last_id;

    int fd_int;
    void* mapped;

    bool write(Tuple<T> tuple) {
        auto size_tuple = sizeof(std::int64_t) + sizeof(T);
        auto write_ret = ::write(fd_int, &tuple, size_tuple);
        // write gibt die anzahl an geschriebenen bytes zurück. es sollten immer 16 sein.
        if (write_ret != size_tuple) {
            std::cout << "error with writing." << std::endl;
            return false;
        }
        this->last_id = this->last_id + 1;
        auto ret_fsync = fsync(this->fd_int);
        if (ret_fsync != 0) {
            std::cout << std::strerror(errno) << std::endl;
            std::cout << "fsync failed." << std::endl;
            return false;
        }
        return true;
    }


    T* value_at_id(std::int64_t id) {
        auto *mapped_cast = (std::int8_t* ) mapped;
        auto length_tuple = sizeof(std::int64_t) + sizeof(T);
        auto offset = id * length_tuple;
        std::int8_t* address_of_id = mapped_cast + offset;
        std::cout << "addres of id is: " << address_of_id << std::endl;
        auto addres_of_id_cast = (std::int64_t *) address_of_id;
        std::cout << "id is: " << id << " and reading: " << *addres_of_id_cast << std::endl;
        auto address_of_value = addres_of_id_cast + 1;
        T* value = ((T* ) address_of_value);
        return value;
    }

public:
    Column(const std::string& name) {
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

        this->mapped = mmap(nullptr, statbuf.st_size, PROT_READ , MAP_SHARED, fd_int, 0);
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


    void read_all() {
        // TODO make work with templates
        std::cout << "id\tvalue" << std::endl;
        auto length_tuple = sizeof(T) + sizeof(std::int64_t);
        std::cout << "length tuple: " << length_tuple << std::endl;
        auto *mapped_cast = (std::int8_t *) mapped;
        for (int i = 0; i <= last_id; ++i) {
            std::cout << i << "\t";
            std::cout << this->value_at_id(i) << std::endl;
            /*auto addres_of_id = mapped_cast + (i * length_tuple);
            auto id_pointer = (std::int64_t *) addres_of_id;
            std:int64_t id = *id_pointer;
            std::cout << id << "\t";
            auto address_of_value = addres_of_id + sizeof(std::int64_t);
            T value = *((T *) address_of_value);
            std::cout << value  << std::endl;*/
        }
    }

    // TODO make it handle invalid ids
    T read_value(std::int64_t id) {
        return *value_at_id(id);
    }

    std::int64_t write_value(T value) {
        Tuple new_tuple(last_id + 1, value);
        bool worked = this->write(new_tuple);
        if (worked) {
            return this->last_id;
        } else {
            return -1;
        }
    }
};


#endif //COLUMN_STORE_COLUMN_H
