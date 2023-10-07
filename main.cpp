#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "column.h"
#include "table.h"


bool is_number(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

void repl_loop() {
    Column<std::int64_t> column("score");
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
    return 0;
}