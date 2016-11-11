/**
 * Mike Knee 11/11/2016
 *
 * Source code for a program which uses Euler's method to solve a first order
 * differential equation. The differential equation is y' = 1 + y^2. The
 * solution for this system is easily obtained analytically, and so the program
 * is also able to calculate an analytic answer for comparison. 
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>

/**
 * Derivative function provides the value of y' at any point.
 *
 * double x : x input to the function.
 * double y : y input to the function.
 * return : y' at (x, y).
 */
double Derivative(double x, double y);

/**
 * Provides the actual value of y.
 *
 * double x : x to evaluate value of y.
 * return : value of y at x.
 */
double Analytic(double x);

/**
 * Function to perform Euler's method using initial conditions startY and
 * startX, finding the value of y at finalX. Method is performed using
 * specified number of intervals.
 *
 * *d : Function which provides the value of y' at arbitrary point.
 * double startY : initial value of y.
 * double startX : initial value of x.
 * int intervals : number of intervals to use.
 * double finalX : value of x to estimate a value of y for.
 * return : Estimated value of y at finalX.
 */
double Euler(double (*d)(double, double), double startY, double startX, int intervals, double finalX);

/**
 * Main function which specifies initial conditions, and then asks for maximum
 * number of intervals to use. Then applies euler method, using intervals 1 ->
 * specified amount.
 */
int main()
{
	double startY, startX, finalX;

	// Initial conditions.
	startY = 0;
	startX = 0;

	printf("Please input goal time: ");
	std::cin >> finalX;
	
	int intervals;
	printf("Please input no. of intervals: 10^");
	std::cin >> intervals;

	if (intervals > 10000)
	{
		printf("This may take some time...");
	}

	FILE * file;

	file = fopen("euler_out", "w");

	printf("Writing to file 'euler_out'...\n");
	
	double answer;
	// Actual answer for computing error.
	double actual = Analytic(finalX);

	fprintf(file, "%-15s%-20s%-20s%-20s\n", "Intervals", "Result", "Analytic Error", "Width");
	
	for (int i = 0; i <= intervals*2; i++)
	{
		// Compute answer, then write to file using intervals = i.
		answer = Euler(Derivative, startY, startX, (int)std::pow(10, i*0.5), finalX);
		fprintf(file, "%-15i%-20.15f%-20.15f%-20.15f\n", (int)std::pow(10, i*0.5), answer, std::abs((answer-actual)/actual), (finalX - startX)/std::pow(10, i*0.5));
	}

	fclose(file);

	printf("Done!\n");

	return 0;
}

double Derivative(double x, double y)
{
	return 1 + y*y;
}

double Analytic(double x)
{
	// See report for details.
	return std::tan(x);
}

double Euler(double (*d)(double, double), double startY, double startX, int intervals, double finalX)
{
	double h = (finalX-startX)/intervals;

	double y0, y1, x;

	// Initialise.
	y0 = startY;
	x = startX;

	// Invariant: We have moved to point x = startX + i*h.
	for (int i = 0; i != intervals; i++)
	{
		// Apply euler's method.
		y1 = y0 + h*(*d)(x, y0);

		y0 = y1;
		x = x + h;
	}

	return y0;
}
