#include "pch.h" 
#include "CppUnitTest.h"
#include <fstream>
#include <cstdlib>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

TEST_CLASS(CreatorTests) {
public:

    TEST_METHOD(CreatesFileWithOneEmployee) {
        ofstream input("creator_input.txt");
        input << "1 Roflik 15\n";
        input.close();

        remove("empl.bin");

        string cmd = "Creator.exe empl.bin 1 < creator_input.txt";
        int ret = system(cmd.c_str());
        Assert::AreEqual(0, ret);

        ifstream f("empl.bin", ios::binary);
        Assert::IsTrue(f.good());

        employee e{};
        f.read((char*)&e, sizeof(e));
        Assert::AreEqual(1, e.num);
        Assert::AreEqual(string("Roflik"), string(e.name));
        Assert::AreEqual(15.0, e.hours);
    }
};

TEST_CLASS(ReporterTests) {
public:

    TEST_METHOD(GeneratesReport) {
        employee e{ 1, "Roflik", 15.0 };
        ofstream bin("empl.bin", ios::binary);
        bin.write((char*)&e, sizeof(e));
        bin.close();

        remove("report.txt");

        string cmd = "Reporter.exe empl.bin report.txt 15";
        int ret = system(cmd.c_str());
        Assert::AreEqual(0, ret);

        ifstream rep("report.txt");
        Assert::IsTrue(rep.good());

        string line, all;
        while (getline(rep, line)) {
            all += line + "\n";
        }
        Assert::IsTrue(all.find("Roflik") != string::npos);
        Assert::IsTrue(all.find("225") != string::npos); 
    }
};

TEST_CLASS(MainTests) {
public:

    TEST_METHOD(RunsWholePipeline) {
        ofstream creatorIn("creator_input.txt");
        creatorIn << "1 Roflik 15\n";  
        creatorIn.close();

        remove("empl.bin");
        remove("report.txt");

        string creatorCmd = "Creator.exe empl.bin 1 < creator_input.txt";
        int ret1 = system(creatorCmd.c_str());
        Assert::AreEqual(0, ret1);

        ifstream bin("empl.bin", ios::binary);
        Assert::IsTrue(bin.good());
        employee e{};
        bin.read((char*)&e, sizeof(e));
        Assert::AreEqual(1, e.num);
        Assert::AreEqual(string("Roflik"), string(e.name));
        Assert::AreEqual(15.0, e.hours);

        string reporterCmd = "Reporter.exe empl.bin report.txt 15";
        int ret2 = system(reporterCmd.c_str());
        Assert::AreEqual(0, ret2);

        ifstream rep("report.txt");
        Assert::IsTrue(rep.good());

        string line, all;
        while (getline(rep, line)) {
            all += line + "\n";
        }
        Assert::IsTrue(all.find("Roflik") != string::npos);
        Assert::IsTrue(all.find("225") != string::npos); 
    }
};

