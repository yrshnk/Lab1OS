#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "employee.h"

void printBinaryFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file " << filename << "\n";
        return;
    }

    employee emp;
    std::cout << "Binary file contents:\n";
    std::cout << "Num\tName\tHours\n";
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        std::cout << emp.num << "\t" << emp.name << "\t" << emp.hours << "\n";
    }
    inFile.close();
}

void printReportFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening report file " << filename << "\n";
        return;
    }

    std::string line;
    std::cout << "Report contents:\n";
    while (std::getline(inFile, line)) {
        std::cout << line << "\n";
    }
    inFile.close();
}

int main() {
    std::string binFile;
    int n;

    std::cout << "Enter binary file name: ";
    std::cin >> binFile;
    std::cout << "Enter number of records: ";
    std::cin >> n;

    std::string creatorCmd = "Creator.exe " + binFile + " " + std::to_string(n);

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcess(nullptr, creatorCmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        std::cerr << "Failed to create Creator process.\n";
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printBinaryFile(binFile);

    std::string reportFile;
    double rate;
    std::cout << "Enter report file name: ";
    std::cin >> reportFile;
    std::cout << "Enter hourly rate: ";
    std::cin >> rate;

    std::string reporterCmd = "Reporter.exe " + binFile + " " + reportFile + " " + std::to_string(rate);

    if (!CreateProcess(nullptr, reporterCmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        std::cerr << "Failed to create Reporter process.\n";
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printReportFile(reportFile);

    return 0;
}
