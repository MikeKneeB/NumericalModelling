/**
 * Mike Knee 05/11/2016
 *
 * This source code compiles to a program that uses the GSL adaptive ODE
 * solving methods. Similar to question5-2.cpp a driver is specified, but in
 * this program it is allowed to adaptively pick step sizes to produce a result
 * within the desired error boundary.
 *
 * GSL ver 1.16
 */

#include <cstdio>
#include <cmath>
#include <iostream>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

/**
 * Function that gives the derivatives of both v and x. See report for details.
 *
 * double t : Time to evaluate derivatives at. (Not used here, but for more
 * 		complex systems could be.)
 * double y[] : Array corresponding to v and x (y[0] = v, y[1] = x).
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

double ErrorEstimate(const double yInitial[], const double y[]);

/**
 * Main function, creates necessary objects and then allows driver object to
 * find result using adaptive number of steps.
 *
 * We both output the result and make a file containing the value of v, x and t
 * after each interval.
 */
int main()
{
	int * params = 0;
	// Define system as before (see question5-2.cpp).
	gsl_odeiv2_system sys = {Function, Jacobian, 2, params};

	// Define driver as before, except in this case the final two arguments
	// specifying absolute and relative error are relevent, as this is what
	// the driver will use as its benchmark for success. The driver will
	// use adaptive step size, making the steps as large as possible while
	// keeping the errors below the specified amounts.
	gsl_odeiv2_driver * driver = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, 1e-3, 1e-10, 1e-10);

	double t = 0;
	double yInitial[2] = {1.0, 0.0};
	double y[2] = {yInitial[0], yInitial[1]};

	double goal;
	printf("Enter goal time: ");
	std::cin >> goal;

	// Ask driver to apply, store success/failure code in s.
	int s = gsl_odeiv2_driver_apply(driver, &t, goal, y);

	if (s != GSL_SUCCESS)
	{
		printf("Critical failure.\n");
	}
	else
	{
		//Print result.
		printf("V: %-15.15f X: %-15.15f\n", y[0], y[1]);
	}

	// Finished with driver object...
	gsl_odeiv2_driver_free(driver);

	// Create lower level odeiv2 objects, outside of a driver wrapper.
	gsl_odeiv2_step * step = gsl_odeiv2_step_alloc(gsl_odeiv2_step_rk4, 2);
	// Desired precision is 10 s.f.
	gsl_odeiv2_control * control = gsl_odeiv2_control_y_new(1e-10, 1e-10);
	gsl_odeiv2_evolve * evolve = gsl_odeiv2_evolve_alloc(2);

	// Reset, and define some working variables.
	double h = 1;
	int count = 1;
	t = 0;
	y[0] = yInitial[0];
	y[1] = yInitial[1];

	FILE * file;
	file = fopen("phase_adap_gsl_rk_out", "w");

	fprintf(file, "%-10s%-20s%-20s%-20s%-20s%-20s\n", "Interval", "Time", "Result V", "Result X", "Width", "Error Est.");
	printf("Writing output to file 'phase_adap_gsl_rk_out'...\n");

	while (t < goal)
	{
		// Define our starting width as 1, which is expected to change in the first
		// iteration.
		s = gsl_odeiv2_evolve_apply(evolve, control, step, &sys, &t, goal, &h, y);
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

	printf("Done!\n");

	return 0;

}

int Function(double t, const double y[], double f[], void * params)
{
	f[0] = -y[1];
	f[1] = y[0];
	return GSL_SUCCESS;
}

int Jacobian(double t, const double y[], double * dfdy, double dfdt[], void * params)
{
	return GSL_SUCCESS;
}

double ErrorEstimate(const double yInitial[], const double y[])
{
	double eInitial = yInitial[0] * yInitial[0] + yInitial[1] * yInitial[1];
	double e = y[0] * y[0] + y[1] * y[1];
	return std::abs((e - eInitial)/eInitial);
}
