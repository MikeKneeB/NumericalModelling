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
#include <limits>

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
 * Simpsons is a function to calculate the integral of a function given by *f
 * using the simpson's numerical method. It functions similarly to Trapezium
 * shown above.
 *
 * *f : Function to integrate over.
 * lowerBound : Lower bound for the integration.
 * upperBound : Upper bound for the integration.
 * intervals : Number of seperate intervals to use for the calculation.
 * return : Integral of the function *f between upperBound and lowerBound.
 */
double Simpsons(double (*f)(double), double lowerBound, double upperBound, int intervals);

/**
 * Main function for the program, asks for user input for max number of
 * intervals, and then performs the trapezium method in a loop using intervals
 * 1 -> specified intervals. Writes the output to file 'trapezium_output'
 */
int main()
{

	double (*integrationFunction)(double) = Function;

	cout << endl;
	cout << "#######################################################" << endl;
	cout << "## Trapezium and Simpson's rule numerical calculator ##" << endl;
	cout << "#######################################################" << endl << endl;

	cout << "Options:" << endl << "(1)\tTrapezium rule up to a user input number of intervals."
		<< endl << "(2)\tSimpsons rule up to a user input number of intervals."
		<< endl << "Please enter a number: " << flush;

	int choice;

	while (!(cin >> choice) || choice < 1 || choice > 2)
	{
		cout << "Please enter a valid choice: " << flush;
		cin.clear();
		cin.ignore();
	}

	double lowerBound = 0.0, upperBound = 1.0;

	cout << endl << "Please enter lower bound: " << flush;
	cin >> lowerBound;

	cout << "Please enter upper bound: " << flush;
	cin >> upperBound;

	switch (choice)
	{
		case 1:
		{
			int intervals;

			cout << "Please enter the number of intervals to compute the integration over: ";
			cin >> intervals;
			cout << "Writing to file 'trapezium_output'..." << endl;

	
			ofstream outFile;
			outFile.open("trapezium_output");

			outFile << setiosflags(ios::fixed) << setprecision(10) 
			<< left << setw(10) << "Intervals" << "Result" << endl;

			for (int i = 1; i <= intervals; i*=10)
			{
				outFile << setw(10) << i 
				<< Trapezium(integrationFunction, lowerBound, upperBound, i)
				<< endl;
			}
			
			cout << "Done." << endl;
			outFile.close();
			break;
		}
		case 2:
		{
			int intervals;

			cout << "Please enter the number of intervals to compute the integration over: ";
			cin >> intervals;
			cout << "Writing to file 'simpson_output'..." << endl;

	
			ofstream outFile;
			outFile.open("simpson_output");

			outFile << setiosflags(ios::fixed) << setprecision(10) 
			<< left << setw(10) << "Intervals" << "Result" << endl;

			for (int i = 1; i <= intervals; i*=10)
			{
				outFile << setw(10) << i 
				<< Simpsons(integrationFunction, lowerBound, upperBound, i)
				<< endl;
			}
			
			cout << "Done." << endl;
			outFile.close();
			break;
		}
	}	
	return 0;
}

double Function(double x)
{
	return x*sin(30*x)*cos(x);	
}

double Trapezium(double (*f)(double), double lowerBound, double upperBound, int intervals)
{
	double space = (upperBound-lowerBound)/intervals;

	double total = 0.0;

	// Invariant: we have integrated up to position from lowerBound.
	for (int i = 0; i != intervals; i++)	
	{
		// Trapezium rule:
		total += 0.5*((*f)(i*space) + (*f)((i+1)*space))*space;
	}

	return total;
}

double Simpsons(double (*f)(double), double lowerBound, double upperBound, int intervals)
{
	double space = (upperBound - lowerBound)/intervals;

	double total = 0.0;

	//Invariant: we have integrated up to position from lowerBound.
	for (int i = 0; i != intervals; i++)	
	{
		// Simpsons rule:
		total += (space/6)*((*f)(i*space)+4*(*f)((i+0.5)*space)+(*f)((i+1)*space));
	}

	return total;
}
