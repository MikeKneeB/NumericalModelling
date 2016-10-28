#include <gsl/gsl_integration.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdio.h>

using namespace std;

double Function(double x, void * params);

int main()
{

	// Create a workspace for the integration routine to use.
	gsl_integration_workspace * workspace = gsl_integration_workspace_alloc(1000);
	
	// Create doubles to store the result and error estimate for the routine.
	double result;
	double error;

	// Define a gsl_function, and add our defined function 'Function' to it.
	gsl_function function;
	function.function = &Function;

	// Perform the numerical integration estimate, using 41 point Gauss-Konrod
	// rules.
	// Result and error need to be assigned values by the routine and so are
	// passed as reference as &result and &error.
	gsl_integration_qag(&function, 0, 2, 0, 1e-7, 1000, GSL_INTEG_GAUSS41,  workspace, &result, &error);

	// Print result and error.
	printf("Result: %.15f \n", result);
	printf("Error: %.15f \n", error);
	// Print workspace size, this corresponds to the number of runs of the algorithm
	// needed.
	printf("Intervals: %zu\n", workspace->size);

	// Deallocate space from the workspace.
	gsl_integration_workspace_free(workspace);
}

double Function(double x, void * params)
{
	return exp(-x)*sin(x);
}
