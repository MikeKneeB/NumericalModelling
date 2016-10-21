/**
 * Mike Knee
 *
 * Source file for simple program to calculate the integral of a function defined
 * as Function() using the trapezium method. An input is specified and the process is
 * repeated using intervals 1 - input, with the results being output to a file called
 * 'trapezium_output'.
 */
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>

using namespace std;

/**
 * Defines the function we want to integrate over.
 *
 * x : Value input to the function.
 * return : Value of function at point x.
 */
double Function(double x);

/**
 * Numerically calculates the integral of a function pointed to by *f using the
 * trapezium method. Integrates between upperBound and lowerBound, using
 * number of trapeziums specified by intervals. The higher the value of
 * intervals the more precise the answer will be, until a limit defined by the
 * precision of double is reached.
 *
 * *f : Function to integrate over.
 * lowerBound : Lower bound for the integration.
 * upperBound : Upper bound for the integration.
 * intervals : Number of trapeziums to use for the calculation.
 * return : Integral of the function *f between lowerBound and upperBound.
 */
double Trapezium(double (*f)(double), double lowerBound, double upperBound, int intervals);

/**
 * LogTrapezium calculates the value of an integral using the trapezium method
 * in a loop, increasing the interval number logarithmically until it reaches
 * intervals = 10^8. The results are output to a .csv file called
 * "log_output.csv".
 *
 * *f : Function to integrate over.
 * lowerBound : Lower bound for the integration.
 * upperBound : Upper bound for the integration.
 */
void LogTrapezium(double (*f)(double), double lowerBound, double upperBound);

/**
 * IntPow calculates an integer result for an integer raised to an integer
 * power.
 *
 * x : Base.
 * n : Exponent.
 * return : x^n.
 */
int IntPow(int x, int n);

/**
 * Main function for the program, asks for user input for max number of
 * intervals, and then performs the trapezium method in a loop using intervals
 * 1 -> specified intervals. Writes the output to file 'trapezium_output'
 */
int main()
{
	double lowerBound = 0.0, upperBound = 1.0;
	int intervals;

	cout << "Please enter the number of intervals to compute the integration over: ";
	cin >> intervals;
	cout << "Writing to file 'trapezium_output'..." << endl;

	double (*integrationFunction)(double) = Function;

	ofstream outFile;
	outFile.open("trapezium_output");

	outFile << setiosflags(ios::fixed) << setprecision(10) << left << setw(10) << "Intervals" << "Result" << endl;
	for (int i = 1; i <= intervals; i++)
	{
		outFile << setw(10) << i  
		<< Trapezium(integrationFunction, lowerBound, upperBound, i) << endl;
	}

	cout << "Creating very high interval table in 'log_output'..." << endl;

	LogTrapezium(integrationFunction, lowerBound, upperBound);

	cout << "Done!" << endl;

	return 0;
}

double Function(double x)
{
	return exp(-x)*sin(x);	
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

void LogTrapezium(double (*f)(double), double lowerBound, double upperBound)
{
	ofstream outFile;
	outFile.open("log_output");
	outFile << setiosflags(ios::fixed) << setw(12) << left << setprecision(15);

	outFile << "LogInterval" << "Result" << endl;

	// Invariant: we have performed i runs of the trapezium method.
	for (int i = 0; i != 9; i++)
	{
		outFile << setw(12) << i << Trapezium(f, lowerBound, upperBound, IntPow(10, i)) << endl; 
	}
}

int IntPow(int x, int n)
{
	int total = 1;
	//Invariant: we multiplied total by x i times.
	for (int i = 0; i != n; i++)
	{
		total *= x;
	}
	return total;
}
