#include <cstdio>
#include <iostream>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

int Function(double t, const double y[], double f[], void * params);

int Jacobian(double t, const double y[], double * dfdy, double dfdt[], void * params);

int main()
{
	const gsl_odeiv2_step_type * T = gsl_odeiv2_step_rk4;

	int * params;
	gsl_odeiv2_system sys = {Function, Jacobian, 2, params};

	gsl_odeiv2_driver * driver = gsl_odeiv2_driver_alloc_yp_new(&sys, gsl_odeiv2_step_rk4, 1e-3, 1e-10, 1e-10);

	double t = 0;
	double yInitial[2] = {1.0, 0.0};
	double y[2] = {yInitial[0], yInitial[1]};

	double goal;
	printf("Enter goal time: ");
	std::cin >> goal;

	int interval, s;

	printf("Enter number of step: ");
	std::cin >> interval;

	FILE * file;
	file = fopen("gsl_rk_out", "w");

	fprintf(file, "%-10s%-20s%-20s\n", "Interval", "Result V", "Result X");

	for (int i = 1; i <= interval; i++)
	{
		s = gsl_odeiv2_driver_apply_fixed_step(driver, &t, goal/i, i, y);
		fprintf(file, "%-10i%-20.15f%-20.15f\n", i, y[0], y[1]);
		gsl_odeiv2_driver_reset(driver);
		y[0] = yInitial[0];
		y[1] = yInitial[1];
	}

	fclose(file);
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
