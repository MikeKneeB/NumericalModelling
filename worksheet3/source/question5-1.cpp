/**
 * Mike Knee 05/11/2016
 *
 * Source code for a program to estimate the solution to a second order 
 * differential equation (split into two first order ODEs) using the
 * Runge-Kutta method. Includes a basic vector type for simple required
 * vector operations.
 */

#include <cstdio>
#include <cmath>
#include <iostream>

/**
 * Vector struct, useful for storing results and intermediate answers. Used
 * throughout this code as it is more appropriate than both std::vector<> and
 * the array type.
 */
struct Vector
{
	// Two dimensional vector with two double members.
	// Although there is no reason these members couldn't refer to other
	// things throughout this source code vec.one = v and vec.two = x.
	double one;
	double two;

	// Defining basic operations.
	Vector operator+(Vector const& other)
	{
		Vector temp;
		temp.one = one + other.one;
		temp.two = two + other.two;
		return temp;
	}
	Vector operator-(Vector const& other)
	{
		Vector temp;
		temp.one = one - other.one;
		temp.two = two - other.two;
		return temp;
	}

};

// Defining scalar multiplication.
Vector operator*(Vector const& lhs, double rhs)
{ 
	Vector temp;
	temp.one = lhs.one * rhs;
	temp.two = lhs.two * rhs;
	return temp;
}
Vector operator*(double lhs, Vector const& rhs)
{ 
	Vector temp;
	temp.one = lhs * rhs.one;
	temp.two = lhs * rhs.two;
	return temp;
}
Vector operator/(Vector const& lhs, double rhs)
{ 
	Vector temp;
	temp.one = lhs.one / rhs;
	temp.two = lhs.two / rhs;
	return temp;
}

/**
 * Function derivative return vector derivative at arbitrary time and vector y.
 *
 * double t : Time to evaluate derivative.
 * Vector y : v and x values to evaluate derivative at.
 * return : v' and x' evaluated at t and y.
 */
Vector Derivative(double t, Vector y);

/**
 * Analytic returns the analytic solution of the problem at time t.
 *
 * double t : time to evaluate solution.
 * return : value of v and x at t.
 */
Vector Analytic(double t);

/**
 * Function that applies second order Runge-Kutta using specified derivative
 * function. Uses intervals number of intervals.
 *
 * *d : derivative function for 2 dimensional problem.
 * Vector startY : initial conditions for the solution.
 * double startT : start time for initial conditions.
 * int intervals : number of intervals to use in rk method.
 * double finalT : goal time.
 * return : estimate for v & x at time finalT
 */
Vector RungeKuttaSecond(Vector (*d)(double, Vector), Vector startY, double startT, int intervals, double finalT);

/**
 * Main method requests goal time and max no. of intervals then applies rk
 * method and outputs result to file.
 */
int main()
{
	// Initial conditions.
	Vector startY;
	startY.one = 1;
	startY.two = 0;
	double startT = 0;
	
	double finalT;
	printf("Please input value of t to calculate: ");
	std::cin >> finalT;
	
	int intervals;
	printf("Please input no. of intervals: ");
	std::cin >> intervals;

	FILE * file;

	file = fopen("rk_out", "w");
	
	Vector answer;
	// Compute analytic answer for comparison.
	Vector actual = Analytic(finalT);
	
	printf("Writing to file 'rk_out'...\n");

	fprintf(file, "%-10s%-20s%-20s%-20s%-20s\n",
		"Intervals", "Result V", "Result X", "Analytic Error V", "Analytic Error X");
	
	for (int i = 1; i <= intervals; i++)
	{
		// Apply rk i times.
		answer = RungeKuttaSecond(Derivative, startY, startT, i, finalT);
		fprintf(file, "%-10i%-20.15f%-20.15f%-20.15f%-20.15f\n", 
			i, answer.one, answer.two, 
			std::abs((answer.one-actual.one)/actual.one),
			std::abs((answer.two-actual.two)/actual.two));
	}

	fclose(file);

	printf("Done!\n");

	return 0;
}

Vector Derivative(double t, Vector y)
{
	Vector temp;
	temp.one = -y.two;
	temp.two = y.one;
	return temp;
}

Vector Analytic(double t)
{
	// See report.
	Vector temp;
	temp.one = std::cos(t);
	temp.two = std::sin(t);
	return temp;
}

Vector RungeKuttaSecond(Vector (*d)(double, Vector), Vector startY, double startT, int intervals, double finalT)
{
	double h = (finalT - startT)/intervals;		
	
	// Temporary variables.
	Vector k1, k2, y0, y1;
	double t;

	y0 = startY;
	t = startT;

	// Invariant: we have moved to time t = startT + i*h.
	for (int i = 0; i != intervals; i++)
	{
		k1 = (h * (*d)(t, y0));
		k2 = (h * (*d)(t + h/2, y0 + k1/2.0));
		y1 = y0 + k2;

		y0 = y1;
	}

	return y0;
}
