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

class Tuple {
public:
    Tuple(std::int64_t id, std::int64_t value) {
        this->id = id;
        this->value = value;
    }

    std::int64_t id;
    std::int64_t value;
};

class Column  {
    FILE* fd;
    std::string name;

    std::int64_t last_id;

    int fd_int;
    void* mapped;

    bool write(Tuple tuple) {
        auto write_ret = ::write(fd_int, &tuple, 16);
        // write gibt die anzahl an geschriebenen bytes zurück. es sollten immer 16 sein.
        if (write_ret != 16) {
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
        int length = statbuf.st_size / 16;
        // ids start at 0
        this->last_id = length - 1;
    }

    void read_all() {
        std::cout << "id\tvalue" << std::endl;
        auto *mapped_int64 = (std::int64_t *) mapped;
        for (int i = 0; i <= last_id; ++i) {
            std:int64_t id = *(mapped_int64 + (i * 2));
            std::cout << id << "\t";
            std::int64_t value = *(mapped_int64 + 1 + i * 2);
            std::cout << value  << std::endl;
        }
    }

    // TODO make it handle invalid ids
    std::int64_t read_value(std::int64_t id) {
        auto *mapped_int64 = (std::int64_t *) mapped;
        return *(mapped_int64 + 1 + id * 2);
    }

    std::int64_t write_value(std::int64_t value) {
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
