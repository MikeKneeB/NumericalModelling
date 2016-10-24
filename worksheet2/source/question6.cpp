#include <gsl/gsl_integration.h>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

double Function(double x, void * params);

int main()
{
	gsl_integration_workspace * workspace = gsl_integration_workspace_alloc(1000);
	
	double result;
	double error;

	gsl_function function;
	function.function = &Function;


	gsl_integration_qag(&function, 0, 2, 0, 1e-7, 1000, 4,  workspace, &result, &error);

	cout << "Result:          " << result << endl;
	cout << "Estimated error: " << error << endl;

	gsl_integration_workspace_free(workspace);
}

double Function(double x, void * params)
{
	return exp(-x)*sin(x);
}
