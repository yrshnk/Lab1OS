#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
struct employee {
    int num;
    char name[10];
    double hours;
};

int main() {
    string binfile;
    int records;
    string reportfile;
    float payph;

    cout << "Enter the name of bin-file and the number of records: ";
    cin >> binfile;

    try {
        cin >> records;
        if (cin.fail())
            throw invalid_argument("Invalid argument. Must be a number.");
        if (records <= 0)
            throw out_of_range("Out of range. Number of records must be >= 0.");
    }
    catch (const exception& e) {
        cerr << e.what();
        return 1;
    }

    string CreatorPath = "D:\\Lab1OS\\Creator\\x64\\Debug\\Creator.exe";
    string cmdParameters = "\"" + CreatorPath + "\" " + binfile + " " + to_string(records);

    vector<char> cmdLine(cmdParameters.begin(), cmdParameters.end());
    cmdLine.push_back('\0');

    STARTUPINFOA cr;
    PROCESS_INFORMATION startCreator;

    ZeroMemory(&cr, sizeof(STARTUPINFO));
    cr.cb = sizeof(cr);
    ZeroMemory(&startCreator, sizeof(startCreator));

    if (CreateProcessA(CreatorPath.c_str(), cmdLine.data(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &cr, &startCreator)) {
        WaitForSingleObject(startCreator.hProcess, INFINITE);
        CloseHandle(startCreator.hThread);
        CloseHandle(startCreator.hProcess);

        cout << binfile << " contains: " << endl;

        ifstream binf(binfile, ios::binary);
        employee outTemp;
        for (int i = 0; i < records; i++) {
            binf.read(reinterpret_cast<char*>(&outTemp), sizeof(employee));
            cout << outTemp.num << " " << outTemp.name << " " << outTemp.hours << endl;
        }
    }
    else {
        cerr << "Error occured: Failed to execute Creator.exe! " << GetLastError() << endl;
        return 1;
    }

    cout << "Enter the hourly pay rate: ";
    try {
        cin >> payph;

        if (cin.fail()) {
            throw invalid_argument("Invalid argument. Entered value must be a number.");
        }

        if (payph < 0) {
            throw out_of_range("Out of range. Value must be >= 0.");
        }
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    cout << "Enter the name of report file: ";
    cin >> reportfile;

    string ReporterPath = "D:\\Lab1OS\\Reporter\\x64\\Debug\\Reporter.exe";
    cmdParameters = "\"" + ReporterPath + "\" " + binfile + " " + reportfile + " " + to_string(payph);

    cmdLine.assign(cmdParameters.begin(), cmdParameters.end());
    cmdLine.push_back('\0');

    STARTUPINFOA rep;
    PROCESS_INFORMATION startReporter;

    ZeroMemory(&rep, sizeof(STARTUPINFO));
    rep.cb = sizeof(rep);
    ZeroMemory(&startReporter, sizeof(startReporter));

    if (CreateProcessA(ReporterPath.c_str(), cmdLine.data(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &rep, &startReporter)) {
        WaitForSingleObject(startReporter.hProcess, INFINITE);
        CloseHandle(startReporter.hThread);
        CloseHandle(startReporter.hProcess);

        cout << "Reporter has worked successfully. Report result: " << endl;

        ifstream rprt(reportfile);
        string line;
        while (getline(rprt, line)) {
            cout << line << endl;
        }
    }
    else {
        cerr << "Error occured: Failed to execute Reporter.exe! " << GetLastError() << endl;
        return 1;
    }
    return 0;
}