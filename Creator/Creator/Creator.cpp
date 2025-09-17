#include <fstream>
#include <iostream>

using namespace std;

struct employee {
	int num;
	char name[10];
	double hours;
};

int main(int argc, char* argv[]) {
	ofstream binfile(argv[1], ios::binary | ios::out);

	if (!binfile) {
		cout << "Error occured: file opening/creating is failed.";
		return 1;
	}

	int records = atoi(argv[2]);
	employee* empl_list = new employee[records];

	cout << "Fill out the list of employees (number, name, hours): " << endl;
	for (int i = 0; i < records; i++) {
		cout << i + 1 << ". ";
		cin >> empl_list[i].num >> empl_list[i].name >> empl_list[i].hours;
	}

	for (int i = 0; i < records; i++)
		binfile.write(reinterpret_cast<char*>(&empl_list[i]), sizeof(empl_list[i]));

	binfile.close();
	return 0;
}