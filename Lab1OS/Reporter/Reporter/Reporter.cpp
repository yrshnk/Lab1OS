#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;

struct employee {
	int num;
	char name[10];
	double hours;
};

int main(int argc, char* argv[]) {
	ifstream binfile(argv[1], ios::binary);
	ofstream report(argv[2]);
	double pph = atof(argv[3]);

	vector<employee> empl;
	employee temp;

	while (binfile.read(reinterpret_cast<char*>(&temp), sizeof(employee)))
		empl.push_back(temp);

	report << "Report on file " << argv[1] << ": \n";
	report << "Number of employee, name, hours of work, salary \n";

	float salary;
	for (int i = 0; i < empl.size(); i++) {
		salary = empl[i].hours * pph;
		report << empl[i].num << " " << empl[i].name << " " << empl[i].hours << " " << salary << endl;
	}

	report.close();
	return 0;
}