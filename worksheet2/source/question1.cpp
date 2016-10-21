#include <iostream>
#include <iomanip>

using namespace std;

double Function(double x);

double Trapezium(double (*f)(double), double lowerBound, double upperBound, int intervals);

int main()
{
	double lowerBound = 0.0, upperBound = 1.0;
	int intervals;

	cout << "Please enter the number of intervals to compute the integration over: ";
	cin >> intervals;
	cout << endl;

	double (*integrationFunction)(double) = Function;

	cout << setiosflags(ios::fixed) << endl << left << setw(10) << "Intervals" << "Result" << endl;
	for (int i = 1; i <= intervals; i++)
	{
		cout << setw(10) << i  
		<< Trapezium(integrationFunction, lowerBound, upperBound, i) << endl;
	}

	return 0;
}

double Function(double x)
{
	return 1/((1+x)*(1+x));
}

double Trapezium(double (*f)(double), double lowerBound, double upperBound, int intervals)
{
	double space = (upperBound-lowerBound)/intervals;

	double position = lowerBound;
	double total = 0.0;

	// Invariant: we have integrated up to poisition from lowerBound.
	while (position < upperBound)
	{
		total += 0.5*((*f)(position) + (*f)(position+space))*space;
		position += space;
	}

	return total;
}
