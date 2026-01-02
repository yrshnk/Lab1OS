#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "employee.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: Reporter <binary_file> <report_file> <hourly_rate>\n";
        return 1;
    }

    std::string binFile = argv[1];
    std::string reportFile = argv[2];
    double rate = std::stod(argv[3]);

    std::ifstream inFile(binFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file " << binFile << "\n";
        return 1;
    }

    std::vector<employee> employees;
    employee emp;
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        employees.push_back(emp);
    }
    inFile.close();

    std::sort(employees.begin(), employees.end(), [](const employee& a, const employee& b) {
        return a.num < b.num;
    });

    std::ofstream out(reportFile);
    if (!out) {
        std::cerr << "Error opening report file " << reportFile << "\n";
        return 1;
    }

    out << "Отчет по файлу \"" << binFile << "\"\n";
    out << "Num\tName\tHours\tSalary\n";

    for (const auto& e : employees) {
        double salary = e.hours * rate;
        out << e.num << "\t" << e.name << "\t" << e.hours << "\t" << salary << "\n";
    }

    out.close();
    return 0;
}
