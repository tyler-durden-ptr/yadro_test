#include <filesystem>
#include <fstream>
#include <iostream>

#include "src/headers/Runner.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Wrong number of arguments" << std::endl;
        return 1;
    }
    std::ifstream inputFile(argv[1]);
    if (inputFile.fail()) {
        std::cout << "Failed to open file" << std::endl;
        std::cout << "Local path: " << std::filesystem::current_path().c_str() << std::endl;
        return 1;
    }
    runner::run(inputFile, std::cout);
    inputFile.close();
    return 0;
}
