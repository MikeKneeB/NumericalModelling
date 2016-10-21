#include <iostream>
#include <limits>

using namespace std;

int main(int argv, char * argc[]){

	float a = 0.0;
	float b = 1.0;

	while (b != std::numeric_limits<float>::infinity()) {
		a = b;
		b *= 2;
	}
	
	std::cout << "Approx highest float is: " << a << std::endl;

	double c = 0.0;
	double d = 1.0;

	while (d != std::numeric_limits<double>::infinity()) {
		c = d;
		d *= 2;
	}

	std::cout << "Approx highest double is: " << c << std::endl;

	return 0;

}
