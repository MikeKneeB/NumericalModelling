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
 * The AnalyticSolution supplies the answer to the integration
 * of Function over upper and lower bound. For the proof of this formula's
 * correctness please see the report accompanying this source code.
 *
 * lowerBound : Lower bound for the integration.
 * upperBound : Upper bound for the integration.
 * return : Solution of the integration as given by the analytic solution.
 */
double AnalyticSolution(double lowerBound, double upperBound); 

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
 * LogTrapezium calculates the value of an integral using the trapezium method
 * in a loop, increasing the interval number logarithmically until it reaches
 * intervals = 10^8. The results are output to a file called
 * "log_trapezium".
 *
 * *f : Function to integrate over.
 * lowerBound : Lower bound for the integration.
 * upperBound : Upper bound for the integration.
 */
void LogTrapezium(double (*f)(double), double lowerBound, double upperBound);

/**
 * LogSimpsons performs a similar operation to LogTrapezium, but using the
 * Simpson's method instead of the trapezium method. Results are output to
 * "log_simpson".
 *
 * *f : Function to integrate over.
 * lowerBound : Lower bound for the integration.
 * upperBound : Upper bound for the integration.
 */
void LogSimpsons(double (*f)(double), double lowerBound, double upperBound);

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

	double (*integrationFunction)(double) = Function;

	cout << endl;
	cout << "#######################################################" << endl;
	cout << "## Trapezium and Simpson's rule numerical calculator ##" << endl;
	cout << "#######################################################" << endl << endl;

	cout << "Options:" << endl << "(1)\tTrapezium rule up to a user input number of intervals."
		<< endl << "(2)\tTrapezium rule up to 10^8 intervals."
		<< endl << "(3)\tSimpsons rule up to a user input number of intervals."
		<< endl << "(4)\tSimpsons rule up to 10^8 intervals."
		<< endl << "Please enter a number: " << flush;

	int choice;

	while (!(cin >> choice) || choice < 1 || choice > 4)
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

			for (int i = 1; i <= intervals; i++)
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
			cout << "Creating very high interval table in 'log_trapezium'..." << endl;

			LogTrapezium(integrationFunction, lowerBound, upperBound);

			cout << "Done!" << endl;
			break;
		}
		case 3:
		{
			int intervals;

			cout << "Please enter the number of intervals to compute the integration over: ";
			cin >> intervals;
			cout << "Writing to file 'simpson_output'..." << endl;

	
			ofstream outFile;
			outFile.open("simpson_output");

			outFile << setiosflags(ios::fixed) << setprecision(10) 
			<< left << setw(10) << "Intervals" << "Result" << endl;

			for (int i = 1; i <= intervals; i++)
			{
				outFile << setw(10) << i 
				<< Simpsons(integrationFunction, lowerBound, upperBound, i)
				<< endl;
			}
			
			cout << "Done." << endl;
			outFile.close();
			break;
		}
		case 4:
		{
			cout << "Creating very high interval table in 'log_simpson'..." << endl;

			LogSimpsons(integrationFunction, lowerBound, upperBound);

			cout << "Done!" << endl;
			break;
		}
	}
	
	return 0;
}

double Function(double x)
{
	return exp(-x)*sin(x);	
}

double AnalyticSolution(double lowerBound, double upperBound)
{
	return (-exp(-upperBound) * (cos(upperBound) + cos(upperBound)) 
		+ exp(lowerBound) * (cos(lowerBound) + sin(lowerBound)))/2;
}

double Trapezium(double (*f)(double), double lowerBound, double upperBound, int intervals)
{
	double space = (upperBound-lowerBound)/intervals;

	double position = lowerBound;
	double total = 0.0;

	// Invariant: we have integrated up to position from lowerBound.
	while (position < upperBound)
	{
		total += 0.5*((*f)(position) + (*f)(position+space))*space;
		position += space;
	}

	return total;
}

double Simpsons(double (*f)(double), double lowerBound, double upperBound, int intervals)
{
	double space = (upperBound - lowerBound)/intervals;

	double position = lowerBound;
	double total = 0.0;

	//Invariant: we have integrated up to position from lowerBound.
	while (position < upperBound)
	{
		total += (space/6)*((*f)(position)+4*(*f)(position+space/2)+(*f)(position+space));
		position += space;
	}

	return total;
}

void LogTrapezium(double (*f)(double), double lowerBound, double upperBound)
{
	ofstream outFile;
	outFile.open("log_trapezium");
	outFile << setiosflags(ios::fixed) << setw(12) << left << setprecision(15);

	double analyticAnswer = AnalyticSolution(lowerBound, upperBound);

	double temp, tempHalf;

	outFile << "LogInterval" << "LogError" << endl;

	// Invariant: we have performed i runs of the trapezium method.
	for (int i = 0; i != 9; i++)
	{
		temp = log10(abs(Trapezium(f, lowerBound, upperBound, IntPow(10, i)) - analyticAnswer));
		tempHalf = log10(abs(Trapezium(f, lowerBound, upperBound, pow(10, 0.5+i)) - analyticAnswer));

		outFile << setw(12) << i << temp << endl; 
		outFile << setw(12) << setprecision(1) << 0.5+i << setprecision(15) << tempHalf << endl;
	}

	outFile.close();
}

void LogSimpsons(double (*f)(double), double lowerBound, double upperBound)
{
	ofstream outFile;
	outFile.open("log_simpson");
	outFile << setiosflags(ios::fixed) << setw(12) << left << setprecision(15);

	double analyticAnswer = AnalyticSolution(lowerBound, upperBound);

	double temp, tempHalf;

	outFile << "LogInterval" << "LogError" << endl;

	// Invariant: we have performed i runs of the simpson method.
	for (int i = 0; i != 9; i++)
	{
		temp = log10(abs(Simpsons(f, lowerBound, upperBound, IntPow(10, i)) - analyticAnswer));
		tempHalf = log10(abs(Simpsons(f, lowerBound, upperBound, pow(10, 0.5+i)) - analyticAnswer));

		outFile << setw(12) << i << temp << endl; 
		outFile << setw(12) << setprecision(1) << 0.5+i << setprecision(15) << tempHalf << endl;
	}

	outFile.close();

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
