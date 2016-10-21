#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
/**
 * This code outputs the results for a Bisection method run and two
 * Newton-Raphson runs with different initial values. The results
 * are output to csv files in a standard form, for easy graphing.
 * Note that the csv files do not have headers, so please refer to
 * this source code for the column information.
 */
using namespace std;

//Global count for use in outputs
int count = 0;

//Defined function to solve.
double Function(double Input_Value){
	return (Input_Value*Input_Value*Input_Value + 
			7*Input_Value*Input_Value -
			6*Input_Value +
			15);
}

//Defined derivative of function to solve.
double D_Function(double Input_Value){
	return (3*Input_Value*Input_Value +
			14*Input_Value -
			6);
}

/**
 * Bisection function to recursively find the root of an equation using
 * the bisection method.
 *
 * Lower_Bound : Initial lower bound for the bisection algorithm.
 * Upper_Bound : Initial upper bound for the bisection algorithm.
 * Precision : Precision for the output.
 * output : ofstream object to output to.
 * return : The root of the equation to precision specified by Precision.
 */
double Bisection(double Lower_Bound, double Upper_Bound, double Precision, ofstream & output){
	
	//Output is iteration number, absolute error and result at iteration.
	output << count << "," << (Upper_Bound - Lower_Bound)/2 << "," << (Upper_Bound + Lower_Bound)/2 << endl;

	//Base case for the recursion where, so the function stops when the absolute error
	//is less than the specified precision.
	if ((Upper_Bound - Lower_Bound)/2 < Precision){
		return (Upper_Bound + Lower_Bound)/2;
	}
	else {
		double Middle = (Upper_Bound + Lower_Bound)/2;

		if (Function(Middle)*Function(Upper_Bound) < 0){
			count++;
			return Bisection(Middle, Upper_Bound, Precision, output);
		}
		else {
			count++;
			return Bisection(Lower_Bound, Middle, Precision, output);
		}
	}

}

/**
 * Function that uses the Newton-Raphson algorithm recursively to find
 * the root of the Function defined by Function and D_Function.
 *
 * x : Initial guess for the Newton-Raphson method.
 * Precision : Final precision for the output.
 * output : ofstream object to output to.
 * return : The root of the equation, provided x is a valid initial guess.
 */ 	
double Newton_Raphson(double x, double Precision, ofstream & output){

	double result = x - Function(x)/D_Function(x);

	//Output here is iteration number, convergence and result at iteration.
	output << count << "," << abs(result - x) << "," << result << endl;

	//Base case is when the difference between the current result and the final
	//result is less than precision, ie. the function has converged within a
	//certain tolerance. Note this is not the absolute error.
	if (abs(result - x) < Precision){
		return result;
	}
	else {
		count++;
		return Newton_Raphson(result, Precision, output);
	}
}

int main(){
	
	ofstream bisect, nrone, nrtwo;

	bisect.precision(10);
	bisect.open("bisection.csv");
	cout << Bisection(-10, 10, 0.0000001, bisect) << endl;

	count = 0;

	nrone.precision(10);
	nrone.open("newtonraphson2.csv");
	cout << Newton_Raphson(-10, 0.0000001, nrone) << endl;;

	count = 0;

	nrtwo.precision(10);
	nrtwo.open("newtonraphson1.csv");
	cout << Newton_Raphson(10, 0.0000001, nrtwo) << endl;

	bisect.close();
	nrone.close();
	nrtwo.close();

	return 0;
}
