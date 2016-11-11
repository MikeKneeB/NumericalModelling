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
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
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

	Vector() 
	{}

	Vector(double first, double second) :
		one(first), two(second)
	{}

	Vector(const double arr[]) :
		one(arr[0]), two(arr[1])
	{}

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

/**
 * GSLError uses the GSL Runge-Kutta with interval size from 50 up to specified
 * number of intervals. Starts at 50 intervals because the GSL routine doesn't
 * work very well at low intervals. Outputs to file.
 *
 * std::string filename : output filename.
 * Vector startY : initial conditions for the solution.
 * double startT : start time for the initial conditions.
 * int maxIntervals : highest interval number to use.
 * double finalT : goal time.
 */
void GSLError(std::string filename, Vector startY, double startT, int maxIntervals, double finalT);

/**
 * GSLPhase uses the GSL Runge-Kutta method with specified number of intervals.
 * Writes the state of the system to file after each step, in order to produce
 * a phase plot.
 *
 * std::string filename : output filename.
 * Vector startY : initial conditions for the solution.
 * double startT : start time for the initial conditions.
 * int maxIntervals : interval number to use.
 * double finalT : goal time.
 */
void GSLPhase(std::string filename, Vector startY, double startT, int intervals, double finalT);

/**
 * Function to estimate the error using conservation of energy. See report for
 * details.
 *
 * double yInitial[] : initial conditions of the function.
 * double y[] : current values of v and x.
 * return : estimated error from y from yInitial.
 */
double ErrorEstimate(const double yInitial[], const double y[]);

/**
 * This function uses the GSL adaptive ordinary integration procedure. Outputs
 * after each stage, for a phase plot.
 *
 * std::string filename : output filename.
 * Vector startY : initial conditions for the solution.
 * double startT : start time for the initial conditions.
 * double finalT : goal time.
 */
void AdaptiveGSLPhase(std::string filename, Vector startY, double startT, double finalT);

/**
 * Main method requests goal time and max no. of intervals then applies rk
 * method and outputs result to file.
 *
 * return : 0
 */
int main()
{
	bool running = true;

	printf("\n#############################################\n");
	printf("#                                           #\n");
	printf("# Ordinary Differential Equation Calculator #\n");
	printf("#                                           #\n");
	printf("#############################################\n\n");

	while (running)
	{
		// Initial conditions.
		Vector startY;
		startY.one = 1;
		startY.two = 0;
		double startT = 0;
	
		printf("(1) Second order Runge-Kutta error analysis.\n");
		printf("(2) Second order Runge-Kutta phase plot.\n");
		printf("(3) Fourth order Runge-Kutta error analysis (GSL).\n");
		printf("(4) Fourth order Runge-Kutta phase plot (GSL).\n");
		printf("(5) Adaptive fourth order Runge-Kutta phase plot (GSL).\n");
		printf("(6) Quit.\n");

		int choice;
		printf("Please enter a choice: ");

		while (!(std::cin >> choice) || choice < 1 || choice > 6)
		{
			printf("Enter valid choice: ");
			std::cin.clear();
			std::cin.ignore();
		}

		if (choice == 6) break;
	
		double finalT;
		printf("\nPlease input goal time: ");
		std::cin >> finalT;
	
		int intervals;
		printf("Please input no. of intervals: ");
		std::cin >> intervals;

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
			case 4:
				GSLPhase("phase_gsl_out", startY, startT, intervals, finalT);
				break;
			case 5:
				AdaptiveGSLPhase("adap_phase_gsl_out", startY, startT, finalT);
				break;
		}

		printf("Done!\n");
	}

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
	Vector answer;

	FILE * file = fopen(filename.c_str(), "w");
	
	printf("Writing to file %s...\n", filename.c_str());
	fprintf(file, "%-10s%-20s%-20s%-20s%-20s%-20s\n",
		"Intervals", "Result V", "Result X", "Analytic Error V", "Analytic Error X", "Width");
	
	for (int i = 1; i <= maxIntervals; i++)
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

	return;
}

void RungeKuttaPhase(std::string filename, Vector startY, double startT, int intervals, double finalT)
{
	FILE * file = fopen(filename.c_str() ,"w");

	printf("Writing to file %s...\n", filename.c_str());

	fprintf(file, "%-10s%-20s%-20s%-20s%-20s\n",
		"Interval", "Time", "Result V", "Result X", "Width");

	// Initial conditions.
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

	int s;

	// Initial conditions from startY
	double yInitial[2] = {0,0};
	startY.ArrayConvert(yInitial);

	double yActual[2] = {AnalyticV(finalT), AnalyticX(finalT)};

	double t = startT;

	double y[2] = {yInitial[0], yInitial[1]};

	FILE * file = fopen(filename.c_str(), "w");
			
	printf("Writing to file %s...\n", filename.c_str());

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

	fclose(file);

	gsl_odeiv2_driver_free(driver);

	return;
}

void GSLPhase(std::string filename, Vector startY, double startT, int intervals, double finalT)
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

	// Initial conditions from startY.
	double yInitial[2] = {0,0};
	startY.ArrayConvert(yInitial);

	double y[2] = {yInitial[0], yInitial[1]};
	double t = startT;

	// Integer for storing output.
	int s;

	FILE * file = fopen(filename.c_str(), "w");

	printf("Writing to file %s...\n", filename.c_str());

	fprintf(file, "%-10s%-20s%-20s%-20s%-20s\n", "Interval", "Time", "Result V", "Result X", "Width");

	for (int i = 0; i != intervals; i++)
	{
		// Apply one step of size goal/interval.
		s = gsl_odeiv2_driver_apply_fixed_step(driver, &t, (finalT-startT)/intervals, 1, y);

		// Output.
		fprintf(file, "%-10i%-20.15f%-20.15f%-20.15f%-20.15f\n", i, t, y[0], y[1], (finalT-startT)/intervals);

		if (s != GSL_SUCCESS)
		{
			printf("Critical failure.\nUsually caused by width size being too low, try using larger intervals or a smaller goal time.\n");
			break;
		}
	}

	fclose(file);

	gsl_odeiv2_driver_free(driver);

	return;
}

void AdaptiveGSLPhase(std::string filename, Vector startY, double startT, double finalT)
{
	int * params = 0;
	// Define system as before (see question5-2.cpp).
	gsl_odeiv2_system sys = {Function, Jacobian, 2, params};
	
	double t = startT;
	double yInitial[2] = {0,0};
	startY.ArrayConvert(yInitial);
	double y[2] = {yInitial[0], yInitial[1]};

	double absError, anaError;

	int s;

	printf("Please enter desired absolute error boundary: ");
	std::cin >> absError;
	printf("Please enter desired analytic error boundary: ");
	std::cin >> anaError;

	// Create lower level odeiv2 objects, outside of a driver wrapper.
	// Using fourth order rk, 2 dimensions.
	gsl_odeiv2_step * step = gsl_odeiv2_step_alloc(gsl_odeiv2_step_rk4, 2);
	// Use input error amounts. 
	gsl_odeiv2_control * control = gsl_odeiv2_control_y_new(absError, anaError);
	// 2 dimensions.
	gsl_odeiv2_evolve * evolve = gsl_odeiv2_evolve_alloc(2);

	int count = 1;
	// Initial width of 1, will be changed by the function immediately.
	double h = 1;

	FILE * file;
	file = fopen(filename.c_str(), "w");

	fprintf(file, "%-10s%-20s%-20s%-20s%-20s%-20s\n", "Interval", "Time", "Result V", "Result X", "Width", "Error Est.");
	printf("Writing output to file %s...\n", filename.c_str());

	while (t < finalT)
	{
		// Define our starting width as 1, which is expected to change in the first
		// iteration.
		s = gsl_odeiv2_evolve_apply(evolve, control, step, &sys, &t, finalT, &h, y);
		fprintf(file, "%-10i%-20.15f%-20.15f%-20.15f%-20.15f%-20.15f\n", count, t, y[0], y[1], h, ErrorEstimate(yInitial, y));
		count++;
		if (s != GSL_SUCCESS)
		{
			printf("Critical failure.\n");
			break;
		}
	}

	gsl_odeiv2_step_free(step);
	gsl_odeiv2_control_free(control);
	gsl_odeiv2_evolve_free(evolve);

	fclose(file);

	return;
}

double ErrorEstimate(const double yInitial[], const double y[])
{
	// Error estimate using conservation of energy, see report for details.
	double eInitial = yInitial[0] * yInitial[0] + yInitial[1] * yInitial[1];
	double e = y[0] * y[0] + y[1] * y[1];
	return std::abs((e - eInitial)/eInitial);
}
