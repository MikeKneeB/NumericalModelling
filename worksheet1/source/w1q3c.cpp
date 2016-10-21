#include <iostream>

using namespace std;

int main(int argc, char * argv[]){
	
	float unit_float;
	float fluctuation_float = 0.1;
	int count = 0;
	cout.precision(20);

	cout << "Precision\tResult" << endl;
	do {
		unit_float = 1.0;
		unit_float += fluctuation_float;
		count++;
		fluctuation_float /= 10;

		cout << count << "\t\t" << unit_float << endl;
		
	} while (unit_float != 1);

	double unit_double;
	double fluctuation_double = 0.1;
	count = 0;

	cout << endl;
	cout << endl;

	cout << "Precision\tResult" << endl;

	do {
		unit_double = 1.0;
		unit_double += fluctuation_double;
		count++;
		fluctuation_double /= 10;

		cout << count << "\t\t" << unit_double << endl;

	} while (unit_double != 1);

	return 0;
}
