#include <stdio.h>
#include <cmath>
#include <gsl/gsl_integration.h>

#define PI 3.14159265359

double Function(double x, void * params);

int main()
{

	// Allocate a table for qawo integration.
	// Here we specify omega as 30.0 and use the GSL_INTEG_SINE option, which means in our
	// function Function we do not need to include the sin(30x) part, as it is contained in the
	// table defined here. This is an important point for the gsl_integration_qawo routine.
	// We also define the length to integrate over here in the table, so only the start
	// point needs to be specified when we call the actual routine.
	gsl_integration_qawo_table * table = gsl_integration_qawo_table_alloc(30.0, 2*PI, GSL_INTEG_SINE, 1000);
	// Allocate workspace as before.
	gsl_integration_workspace * workspace = gsl_integration_workspace_alloc(1000);

	double result, error;
	
	gsl_function function;
	function.function = &Function;

	gsl_integration_qawo(&function, 0, 1e-7, 0, 1000, workspace, table, &result, &error);  

	printf("Result: %.15f \n", result);
	printf("Error: %.15f \n", error);
	printf("Intervals: %zu\n", workspace->size);

	gsl_integration_qawo_table_free(table);
	gsl_integration_workspace_free(workspace);
} 

double Function(double x, void * params)
{
	// Function does not include sin(30x) factor, as this is included in the
	// gsl_integration_qawo_table.
	return x*cos(x);
}
