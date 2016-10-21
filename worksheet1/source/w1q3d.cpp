#include <iostream>

using namespace std;

template<typename precision> precision Sum_Up(int N) {

	precision n = 1;
	precision sum = 0;

	for (int i = 0; i < N; i++) {
		sum += 1/n;
		n++;
	}
	
	return sum;

}

template<typename precision> precision Sum_Down(int N) {

	precision n = N;
	precision sum = 0;

	for (int i = 0; i < N; i++) {
		sum += 1/n;
		n--;
	}

	return sum;

}

int main(int argc, char * argv[]){

	cout.precision(20);
	int N;
	cout << "Please enter a value of N: ";
	cin >> N;
	cout << endl;

	cout << "Sum up float: " << Sum_Up<float>(N) << endl;
	cout << "Sum down float: " << Sum_Down<float>(N) << endl << endl;

	cout << "Sum up	double: " << Sum_Up<double>(N) << endl;
	cout << "Sum down double: " << Sum_Down<double>(N) << endl;

	return 0;

}
