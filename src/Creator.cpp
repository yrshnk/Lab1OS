#include <iostream>
#include <fstream>
#include <string>
#include "employee.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: Creator <binary_filename> <number_of_records>\n";
        return 1;
    }

    std::string filename = argv[1];
    int n = std::stoi(argv[2]);

    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file " << filename << "\n";
        return 1;
    }

    employee emp;
    for (int i = 0; i < n; ++i) {
        std::cout << "Record " << i + 1 << ":\n";
        std::cout << "Num: ";
        std::cin >> emp.num;
        std::cout << "Name: ";
        std::cin >> emp.name;
        std::cout << "Hours: ";
        std::cin >> emp.hours;

        outFile.write(reinterpret_cast<char*>(&emp), sizeof(emp));
    }

    outFile.close();
    return 0;
}
