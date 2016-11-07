/**
 * Mike Knee 05/11/2016
 *
 * This source file is for a program which uses the GSL ODE libraries to solve
 * the simple harmonic oscillator differential equations.
 *
 * Here fixed step sizes are used, as opposed to allowing the driver object to
 * dynamically choose step size.
 *
 * GSL ver 1.16
 *
 */

#include <cstdio>
#include <iostream>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

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
 * Main method, creates necessary objects and then loops applying fixed steps
 * from 1 to use specified number.
 */
int main()
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

	//Initial conditions.
	double tInitial = 0;
	double yInitial[2] = {1.0, 0.0};
	double t = tInitial;
	double y[2] = {yInitial[0], yInitial[1]};

	double goal;
	printf("Enter goal time: ");
	std::cin >> goal;

	int interval, s;

	printf("Enter number of steps: ");
	std::cin >> interval;

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
			file = fopen("gsl_rk_out", "w");

			printf("Writing to file 'gsl_rk_out'...\n");

			fprintf(file, "%-10s%-20s%-20s%-20s\n", "Interval", "Result V", "Result X", "Width");

			for (int i = 1; i <= interval; i++)
			{
				// Apply steps of size (goal/i) i times, store result in y.
				// This means we will always get to the goal in i steps, as our
				// initial condition for t is t=0.
				s = gsl_odeiv2_driver_apply_fixed_step(driver, &t, (goal-tInitial)/i, i, y);
				// Print output before error check, as result may give some
				// indication about failure...
				fprintf(file, "%-10i%-20.15f%-20.15f%-20.15f\n", i, y[0], y[1], (goal-tInitial)/i);
				//Check for error, and stop looping if something goes wrong.
//				if (s != GSL_SUCCESS)
//				{
//					printf("Critical failure.\n");
//					break;
//				}
				gsl_odeiv2_driver_reset(driver);
				y[0] = yInitial[0];
				y[1] = yInitial[1];
			}
			break;

		case 2:
			file = fopen("phase_gsl_rk_out", "w");

			printf("Writing to file 'phase_gsl_rk_out'...\n");

			fprintf(file, "%-10s%-20s%-20s%-20s%-20s\n", "Interval", "Time", "Result V", "Result X", "Width");

			for (int i = 0; i != interval; i++)
			{
				// Apply one step of size goal/interval.
				s = gsl_odeiv2_driver_apply_fixed_step(driver, &t, (goal-tInitial)/interval, 1, y);

				fprintf(file, "%-10i%-20.15f%-20.15f%-20.15f%-20.15f\n", i, t, y[0], y[1], (goal-tInitial)/interval);

				if (s != GSL_SUCCESS)
				{
					printf("Critical failure");
					break;
				}
			}
			break;

	}

	fclose(file);

	printf("Done!\n");

	// Free space.
	gsl_odeiv2_driver_free(driver);

	return 0;

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
