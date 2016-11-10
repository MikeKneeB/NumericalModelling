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
 * the array type. (Neither have scalar multiplication or addition which is
 * invaluable.)
 */
struct Vector
{
	// Two dimensional vector with two double members.
	// Although there is no reason these members couldn't refer to other
	// things throughout this source code vec.one = v and vec.two = x.
	double one;
	double two;

	// Defining basic arithmetic operations.
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

	void ArrayConvert(double out[])
	{
		out[0] = one;
		out[1] = two;
		return;
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
 * Function that performs one step of the second order Runge-Kutta method.
 *
 * *d : derivative function for 2 dimensional problem.
 * Vector y0 : value of y at start of step.
 * double t0 : value of t at start of step.
 * double h : width of step.
 * return : estimate value of y at t0 + h.
 */
Vector RungeKuttaStep(Vector (*d)(double, Vector), Vector y0, double t0, double h);

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
 * RungeKuttaError runs the Runge-Kutta method maxIntervals number of times,
 * increasing the intervals used by 1 until maxIntervals is reached. Outtput
 * is written to a file.
 *
 * std::string filename : output filename.
 * Vector startY : initial conditions for the solution.
 * double startT : start time for the intial conditions.
 * int maxIntervals : highest interval number to use.
 * double finalT : goal time.
 */
void RungeKuttaError(std::string filename, Vector startY, double startT, int maxIntervals, double finalT);

/**
 * RungeKuttaPhase outputs the results for the Runge-Kutta method to a file
 * after each single step of the algorithm. This is in order to produce a
 * phase plot of the solution.
 *
 * std::string filename : output filename.
 * Vector startY : initial conditions for the solution.
 * double startT : start time for the intial conditions.
 * int maxIntervals : highest interval number to use.
 * double finalT : goal time.
 */
void RungeKuttaPhase(std::string filename, Vector startY, double startT, int intervals, double finalT);

/**
 * AnalyticV returns the analytic solution for v at time t.
 *
 * double t : time to evaluate solution.
 * return : value of v at t.
 */
double AnalyticV(double t);

/**
 * AnalyticX returns the analytic solution for x at time t.
 *
 * double t : time to evaluate solution.
 * return : value of v at t.
 */
double AnalyticX(double t);

/**
 * Function that gives the derivatives of both v and x. See report for details.
 *
 * double t : Time to evaluate derivatives at. (Not used here, but for more
 * 		complex systems could be.)
 * double y[] : Array corresponding to v and x (y[0] = v, y[1] = x.
 * double f[] : Array to store the values of derivatives after function has
 * 		been called (f[0] = v', f[1] = x').
 * void * params : Parameters for the function required by GSL, unused here.
 * return : Status code, GSL_SUCCESS indicating success. As nothing complicated
 * 		happens we return this every time.
 */
int Function(double t, const double y[], double f[], void * params);

/**
 * Jacobian function pointer required by GSL routines, but unused.
 */
int Jacobian(double t, const double y[], double * dfdy, double dfdt[], void * params);


void GSLError(std::string filename, Vector startY, double startT, int maxIntervals, double finalT);

void GSLPhase(std::string filename, Vector startY, double startT, int intervals, double finalT);

/**
 * Main method requests goal time and max no. of intervals then applies rk
 * method and outputs result to file.
 */
int main()
{
	// Initial conditions.
	Vector startY, answer;
	startY.one = 1;
	startY.two = 0;
	double startT = 0;
	
	double finalT;
	printf("Please input goal time: ");
	std::cin >> finalT;
	
	int intervals;
	printf("Please input no. of intervals: ");
	std::cin >> intervals;

	FILE * file;

	int choice;
	printf("Please enter 1 for error analysis, 2 for phase plot: ");

	while (!(std::cin >> choice) || choice < 1 || choice > 2)
	{
		printf("Enter valid choice: ");
		std::cin.clear();
		std::cin.ignore();
	}
	
	switch (choice)
	{
		case 1:
			RungeKuttaError("rk_out", startY, startT, intervals, finalT);
			break;
		case 2:
			RungeKuttaPhase("phase_rk_out", startY, startT, intervals, finalT);
			break;
		case 3:
			GSLError("gsl_out", startY, startT, intervals, finalT);
			break;
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
	// See report for details of the analytic solution.
	Vector temp;
	temp.one = std::cos(t);
	temp.two = std::sin(t);
	return temp;
}

Vector RungeKuttaStep(Vector (*d)(double, Vector), Vector y0, double t0, double h)
{
	Vector k1, k2, y1;

	// Intermediate r-k values.
	k1 = (h * (*d)(t0, y0));
	k2 = (h * (*d)(t0 + h/2, y0 + k1/2.0));

	y1 = y0 + k2;

	return y1;
}

Vector RungeKuttaSecond(Vector (*d)(double, Vector), Vector startY, double startT, int intervals, double finalT)
{
	double h = (finalT - startT)/intervals;		
	
	// Temporary variables.
	Vector y;
	double t;

	y = startY;
	t = startT;

	// Invariant: we have moved to time t = startT + i*h.
	for (int i = 0; i != intervals; i++)
	{
		y = RungeKuttaStep(d, y, t, h);
		t += h;
	}

	return y;
}

void RungeKuttaError(std::string filename, Vector startY, double startT, int maxIntervals, double finalT)
{
	Vector actual = Analytic(finalT);

	file = fopen(filename, "w");
	
	printf("Writing to file %s...\n", filename);
	fprintf(file, "%-10s%-20s%-20s%-20s%-20s%-20s\n",
		"Intervals", "Result V", "Result X", "Analytic Error V", "Analytic Error X", "Width");
	
	for (int i = 1; i <= intervals; i++)
	{
		// Apply rk i times.
		answer = RungeKuttaSecond(Derivative, startY, startT, i, finalT);
		fprintf(file, "%-10i%-20.15f%-20.15f%-20.15f%-20.15f%-20.15f\n", 
		i, answer.one, answer.two, 
		std::abs((answer.one-actual.one)/actual.one),
		std::abs((answer.two-actual.two)/actual.two),
		(finalT - startT)/i);
	}

	fclose(file);

	printf("Done!");
	
	return;
}

void RungeKuttaPhase(std::string filename, Vector startY, double startT, int intervals, double finalT)
{
	file = fopen("phase_rk_out", "w");

	printf("Writing to file 'phase_rk_out'...\n");

	fprintf(file, "%-10s%-20s%-20s%-20s%-20s\n",
		"Interval", "Time", "Result V", "Result X", "Width");

	double t = startT;
	double h = (finalT - startT)/intervals;
	Vector y = startY;

	for (int i = 0; i != intervals; i++)
	{
		// Apply rk 1 time.
		fprintf(file, "%-10i%-20.15f%-20.15f%-20.15f%-20.15f\n", i, t, y.one, y.two, h);
		y = RungeKuttaStep(Derivative, y, t, h);
		// Increment t.
		t += h;
	}

	fclose(file);

	printf("Done!");

	return;
}

double  AnalyticV(double t)
{
	// See report.
	return std::cos(t);
}

double AnalyticX(double t)
{
	return std::sin(t);
}

int Function(double t, const double y[], double f[], void * params)
{
	// See report for details of this equation.
	f[0] = -y[1];
	f[1] = y[0];
	return GSL_SUCCESS;
}

int Jacobian(double t, const double y[], double * dfdy, double dfdt[], void * params)
{
	return GSL_SUCCESS;
}

void GSLError(std::string filename, Vector startY, double startT, int maxIntervals, double finalT)
{
	int * params = 0;
	// Define system for the ODE, with Function, Jacobian and params.
	// Third argument '2' corresponds to the dimensions of the system
	// which in this case is 2 (v & x).
	gsl_odeiv2_system sys = {Function, Jacobian, 2, params};

	// Create driver object for the system. Driver object is a higher level
	// wrapper for all the various GSL functions related to ODE solving.
	// This wrapper drastically reduces code complexity for our program.
	//
	// We pass in our system as the first parameter and then specify which
	// method to use, in this case fourth order Runge-Kutta. Following this
	// is the initial step size and the desired absolute and relative error
	// respectively. In our case fixed step sizes are used so these final
	// two parameters will not be required by the routine, and could be set
	// to any value.
	gsl_odeiv2_driver * driver = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, 1e-3, 1e-10, 1e-10);

	double yInitial[2];
	startY.ArrayConvert(yInitial);
	double yActual[2] = {AnalyticV(goal), AnalyticX(goal)};

	double y[2] = {yInitia[0], yInitial[2]};

	file = fopen(filename, "w");

			
	printf("Writing to file %s...\n", filename);

	fprintf(file, "%-10s%-20s%-20s%-20s%-20s%-20s\n", "Interval", "Result V", "Result X", "Error V", "Error X", "Width");
	
	// Loop starts at 50, as the GSL routine fails at very large interval sizes.
	// Starting from 50 still gives useful results, but prevents failures.
	for (int i = 50; i <= maxIntervals; i++)
	{
		// Apply steps of size (goal/i) i times, store result in y.
		// This means we will always get to the goal in i steps, as our
		// initial condition for t is t=0.
		s = gsl_odeiv2_driver_apply_fixed_step(driver, &t, (finalT-startT)/i, i, y);
		// Print output before error check, as result may give some
		// indication about failure...
		fprintf(file, "%-10i%-20.15f%-20.15f%-20.15f%-20.15f%-20.15f\n", i, y[0], y[1], std::abs((y[0] - yActual[0])/yActual[0]), std::abs((y[1] - yActual[1])/yActual[1]), (finalT-startT)/i);
		//Check for error, and stop looping if something goes wrong.
		if (s != GSL_SUCCESS)
		{
			printf("Critical failure.\n");
			break;
		}
		gsl_odeiv2_driver_reset(driver);
		y[0] = yInitial[0];
		y[1] = yInitial[1];
	}

	printf("Done!");

	fclose(file);

	gsl_odeiv2_driver_free(driver);
}
