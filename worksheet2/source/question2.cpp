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
 * PrecSimpsons is a function to calculate an answer to an integral using the
 * Simpson rule, to a prescribed number of significant figures. It uses the
 * Simpsons function described above, and thhe AnalyticSolution function to
 * compare the given answer against. If the difference between the answer
 * and the analytic answer is not under 10-sf the function repeats with more
 * intervals.
 *
 * *f : Function to integrate over.
 * lowerBound : Lower bound for the integration.
 * upperBound : Upper bound for the integration.
 * sf : Significant figures for the answer to be.
 * return : Integral of the function *f between upperBound and lowerBound,
 * 	to sf significant figures.
 */
double PrecSimpsons(double (*f)(double), double lowerBound, double upperBound, int sf);

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
		<< endl << "(4)\tSimpsons rule up to a specified precision."
		<< endl << "(5)\tSimpsons rule up to 10^8 intervals."
		<< endl << "Please enter a number: " << flush;

	int choice;

	while (!(cin >> choice) || choice < 1 || choice > 5)
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

	cout << setprecision(15) << "Analytic Solution: " << AnalyticSolution(lowerBound, upperBound) << endl;

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
			int sf;
			cout << "Please enter desired number of significant figures (int): ";
			cin >> sf;
			PrecSimpsons(integrationFunction, lowerBound, upperBound, sf);
			break;
		}
		case 5:
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
	return (-exp(-upperBound) * (cos(upperBound) + sin(upperBound)) 
		+ exp(-lowerBound) * (cos(lowerBound) + sin(lowerBound)))/2;
}

double Trapezium(double (*f)(double), double lowerBound, double upperBound, int intervals)
{
	double space = (upperBound-lowerBound)/intervals;

	double total = 0.0;

	// Invariant: we have integrated up to position from lowerBound.
	for (int i = 0; i != intervals; i++)	
	{
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
		total += (space/6)*((*f)(i*space)+4*(*f)((i+0.5)*space)+(*f)((i+1)*space));
	}

	return total;
}

void LogTrapezium(double (*f)(double), double lowerBound, double upperBound)
{
	ofstream outFile;
	outFile.open("log_trapezium");
	outFile << setiosflags(ios::fixed) << setw(15) << left << setprecision(15);

	double analyticAnswer = AnalyticSolution(lowerBound, upperBound);

	double temp, tempError;

	outFile << "LogInterval" << setw(22) << "LogError" << "Result" << endl;

	// Invariant: we have performed i runs of the trapezium method.
	for (int i = 0; i != 18; i++)
	{
		temp = log10(abs((Trapezium(f, lowerBound, upperBound, pow(10, i/2.0)) - analyticAnswer)/analyticAnswer));
		tempError = log10(abs((temp - analyticAnswer)/analyticAnswer));
		//tempHalf = log10(abs((Trapezium(f, lowerBound, upperBound, pow(10, 0.5+i)) - analyticAnswer)/analyticAnswer));

		outFile << setw(15) << setprecision(1) << i/2.0 << setprecision(15) 
			<< setw(22) << temp << tempError << endl;
//		outFile << setw(15) << setprecision(1) << pow(10, 0.5+i) << setprecision(15) << tempHalf << endl;
	}

	outFile.close();
}

void LogSimpsons(double (*f)(double), double lowerBound, double upperBound)
{
	ofstream outFile;
	outFile.open("log_simpson");
	outFile << setiosflags(ios::fixed) << setw(15) << left << setprecision(15);

	double analyticAnswer = AnalyticSolution(lowerBound, upperBound);

	double temp, tempError;

	outFile << "LogInterval" << "LogError" << endl;

	// Invariant: we have performed i runs of the simpson method.
	for (int i = 0; i != 18; i++)
	{
		temp = Simpsons(f, lowerBound, upperBound, pow(10, i/2.0));
		tempError = log10(abs((temp - analyticAnswer)/analyticAnswer));

		outFile << setw(15) << setprecision(1) << i/2.0 << setprecision(15) 
		<< setw(22) << temp << tempError << endl; 
	}

	outFile.close();

}

double PrecSimpsons(double (*f)(double), double lowerBound, double upperBound, int sf)
{
	double answer = Simpsons(f, lowerBound, upperBound, 1);
	double realAnswer = AnalyticSolution(lowerBound, upperBound);

	int count = 1;
	
	while (abs(answer - realAnswer) > pow(10, -sf)){
		count++;
		answer = Simpsons(f, lowerBound, upperBound, count);
	} 

	cout << setprecision(15) << "Result to " << sf << " significant figures: " << answer << endl;
	cout << "Took " << count << " slices." << endl;

	return answer;	
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
