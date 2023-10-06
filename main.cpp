#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


bool already_load_columns = true;

void map_file(std::string name) {
    // TODO make this have configurable path and create them if it doesnt exists yet.
    int fd;
    fd = open("../store/score", O_RDWR);
    if (fd < 0) {
        std::cout << "couldnt open file" << std::endl;
        std::cout << errno << std::endl;
        std::cout << std::strerror(errno) << std::endl;
        return;
    }
    std::cout << fd << std::endl;
    std::cout << "Hello, World!" << std::endl;
    struct stat statbuf;
    int err = fstat(fd, &statbuf);
    void *mapped = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        std::cout << std::strerror(errno) << std::endl;
        std::cout << "failed to mmap " << std::endl;
        return;
    }
    auto *mapped_int64 = (std::int64_t *) mapped;
    auto length = statbuf.st_size;
    // TODO make the code handle tuples with other datatypes
    // 1 id + 1 value is 8 + 8 = 16 bytes
    auto iterations = statbuf.st_size / 16;
    std::cout << "reading from file" << std::endl;
    for (int i = 0; i < iterations; ++i) {
        std::cout << "first iteration" << std::endl;
        std:int64_t id = *(mapped_int64 + (i * 16));
        std::cout << id << std::endl;
        std::int64_t value = *(mapped_int64 + 1 + i * 16);
        std::cout << value  << std::endl;
    }

}

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

    void write(Tuple tuple) {
        auto write_ret = ::write(fd_int, &tuple, 16);
        std::cout << "write_ret : " << write_ret << std::endl;
        if (write_ret != 16) {
            std::cout << "error with writing." << std::endl;
        }
        this->last_id = this->last_id + 1;
        auto ret_fsync = fsync(this->fd_int);
        if (ret_fsync != 0) {
            std::cout << std::strerror(errno) << std::endl;
            std::cout << "fsync failed." << std::endl;
        }
    }



public:
    Column(const std::string& name) {
        this->fd = fopen(("../store/" + name).c_str(), "a+");
        // TODO apparently SEEK_END is not required, so replace this to using the length of the file
        this->name = name;
        // -1 = not yet calculated
        this->last_id = -1;


        this->fd_int = open("../store/score", O_RDWR | O_APPEND);
        struct stat statbuf;
        int error = fstat(this->fd_int, &statbuf);
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
        auto *mapped_int64 = (std::int64_t *) mapped;
        for (int i = 0; i <= last_id; ++i) {
            std:int64_t id = *(mapped_int64 + (i * 2));
            std::cout << id << "\t";
            std::int64_t value = *(mapped_int64 + 1 + i * 2);
            std::cout << value  << std::endl;
        }
    }

    void write_value(std::int64_t value) {
        Tuple new_tuple(last_id + 1, value);
        this->write(new_tuple);
    }
};

bool is_number(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

void repl_loop() {
    Column column("score");
    std::cout << "schreibe i um einen wert zu schreiben, schreibe r um alle gespeicherten werte und ihre ids auszugeben." << std::endl;
    while (true) {
        std::string repl_input;
        std::cin >> repl_input;
        if (repl_input == "r") {
            column.read_all();
        }
        if (repl_input == "i") {
            std::cout << "gebe den zu speichernden wert ein." << std::endl;
            std::string number_input;
            std::cin >> number_input;
            std::int64_t input_as_num = stol(number_input);
            column.write_value(input_as_num);
        }
    }
}

int main() {
    repl_loop();
    if (not already_load_columns) {
        FILE *fd;
        fd = fopen("../store/score", "r+");
        Tuple test_tuple(0, 5);
        size_t ret;
        ret = fwrite(&test_tuple, 16, 1, fd);
        std::cout << ret << std::endl;
        fclose(fd);
    }
    map_file("score");
    return 0;
}