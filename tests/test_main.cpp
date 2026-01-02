#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include "E:/Lab1OS/src/employee.h"

void writeEmployees(const std::string& filename, const std::vector<employee>& emps) {
    std::ofstream out(filename, std::ios::binary);
    for (const auto& e : emps) {
        out.write(reinterpret_cast<const char*>(&e), sizeof(e));
    }
}

std::vector<employee> readEmployees(const std::string& filename) {
    std::vector<employee> emps;
    std::ifstream in(filename, std::ios::binary);
    employee e;
    while (in.read(reinterpret_cast<char*>(&e), sizeof(e))) {
        emps.push_back(e);
    }
    return emps;
}

std::vector<std::string> readTextFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream in(filename);
    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }
    return lines;
}

TEST(CreatorReporterTest, WriteAndReadBinaryFile) {
    std::string binFile = "test_employees.dat";
    std::vector<employee> emps = {
        {101, "John", 40},
        {102, "Alice", 35}
    };

    writeEmployees(binFile, emps);
    auto loaded = readEmployees(binFile);

    ASSERT_EQ(loaded.size(), emps.size());
    for (size_t i = 0; i < emps.size(); ++i) {
        EXPECT_EQ(loaded[i].num, emps[i].num);
        EXPECT_STREQ(loaded[i].name, emps[i].name);
        EXPECT_DOUBLE_EQ(loaded[i].hours, emps[i].hours);
    }
}

TEST(CreatorReporterTest, ReporterGeneratesReport) {
    std::string binFile = "test_employees.dat";
    std::string reportFile = "test_report.txt";
    double rate = 10.0;

    std::vector<employee> emps = {
        {102, "Alice", 35},
        {101, "John", 40}
    };
    writeEmployees(binFile, emps);

    std::ifstream inFile(binFile, std::ios::binary);
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
    out << "Отчет по файлу \"" << binFile << "\"\n";
    out << "Num\tName\tHours\tSalary\n";
    for (const auto& e : employees) {
        out << e.num << "\t" << e.name << "\t" << e.hours << "\t" << e.hours * rate << "\n";
    }
    out.close();

    auto lines = readTextFile(reportFile);
    ASSERT_EQ(lines.size(), employees.size() + 2); // заголовок + шапка + 2 записи
    EXPECT_EQ(lines[1], "Num\tName\tHours\tSalary");
    EXPECT_EQ(lines[2], "101\tJohn\t40\t400");
    EXPECT_EQ(lines[3], "102\tAlice\t35\t350");
}

TEST(CreatorReporterEdgeCases, EmptyBinaryFile) {
    std::string binFile = "empty.dat";
    std::ofstream out(binFile, std::ios::binary);
    out.close();

    auto loaded = readBinaryFile(binFile);
    EXPECT_TRUE(loaded.empty());
}

TEST(CreatorReporterEdgeCases, ZeroRecords) {
    std::string binFile = "zero.dat";
    std::ofstream out(binFile, std::ios::binary);
    out.close();

    auto loaded = readBinaryFile(binFile);
    EXPECT_EQ(loaded.size(), 0);
}

TEST(CreatorReporterEdgeCases, LongNameTruncation) {
    std::string binFile = "longname.dat";
    employee e = { 123, "VeryLongNameExceed", 20 };
    std::ofstream out(binFile, std::ios::binary);
    out.write(reinterpret_cast<char*>(&e), sizeof(e));
    out.close();

    auto loaded = readBinaryFile(binFile);
    EXPECT_EQ(loaded[0].num, 123);
    EXPECT_STREQ(loaded[0].name, "VeryLongNa"); // обрезается до 10 символов
}

TEST(IntegrationTest, CreatorReporterProcessIntegration) {
    std::string binFile = "integration_employees.dat";
    std::string reportFile = "integration_report.txt";

    std::ofstream tmpInput("tmp_input.txt");
    tmpInput << "101 John 40\n102 Alice 35\n";
    tmpInput.close();

    std::string creatorCmd = "cmd /c type tmp_input.txt | Creator.exe " + binFile + " 2";
    ASSERT_TRUE(runProcess(creatorCmd));

    auto employees = readBinaryFile(binFile);
    ASSERT_EQ(employees.size(), 2);
    EXPECT_EQ(employees[0].num, 101);
    EXPECT_STREQ(employees[0].name, "John");
    EXPECT_DOUBLE_EQ(employees[0].hours, 40);
    EXPECT_EQ(employees[1].num, 102);
    EXPECT_STREQ(employees[1].name, "Alice");
    EXPECT_DOUBLE_EQ(employees[1].hours, 35);

    std::string reporterCmd = "Reporter.exe " + binFile + " " + reportFile + " 15";
    ASSERT_TRUE(runProcess(reporterCmd));

    auto reportLines = readTextFile(reportFile);
    EXPECT_EQ(reportLines.size(), 4); // заголовок + шапка + 2 строки
    EXPECT_EQ(reportLines[2], "101\tJohn\t40\t600");
    EXPECT_EQ(reportLines[3], "102\tAlice\t35\t525");
}