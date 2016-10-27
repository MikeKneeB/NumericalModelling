#include <stdio.h>
#include <cmath>
#include <gsl/gsl_integration.h>

#define PI 3.14159265359

double Function(double x, void * params);

int main()
{

	gsl_integration_qawo_table * table = gsl_integration_qawo_table_alloc(30.0, 2*PI, GSL_INTEG_SINE, 1000);
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
	return x*cos(x);
}
