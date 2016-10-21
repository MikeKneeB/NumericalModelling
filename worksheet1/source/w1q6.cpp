#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

//Global constant
#define PI 3.141592653589793

//Global variables for calculations
double m = 5.5, g = 9.81, k = 850, L = 0.6;
int count = 0;

//Global variable for output formatting
int w = 15;

/**
 * Function is the function to find the root of using the bisection method
 * it simply returns the root.
 *
 * Input_Value : x value to evaluate the function at.
 * return : The value of the function at x.
 */
double Function(double Input_Value){
	return (tan(Input_Value) - sin(Input_Value) - (m*g)/(k*L));
}

/**
 * The Bisection function is a recursive function that performs the bisection
 * method algorithm to find the root of a function. In this case it uses the
 * function Function defined earlier in this source code.
 *
 * Lower_Bound : Initial lower bound for the bisection method.
 * Upper_Bound : Initial upper bound for the bisection method.
 * Precision : Final precision of the return value.
 * return : Root of the function defined by Function between Lower_Bound and
 * 	Upper_Bound to precision Precision.
 */
double Bisection(double Lower_Bound, double Upper_Bound, double Precision){
	
	cout << setw(w) << count << setw(w) << (Upper_Bound - Lower_Bound)/2 << setw(w) << (Upper_Bound + Lower_Bound)/2 << endl;

	//Base case when difference between upper and lower bound over 2 (ie. absolute error)
	//is less than precision.
	if ((Upper_Bound - Lower_Bound)/2 < Precision){
		return (Upper_Bound + Lower_Bound)/2;
	}
	else {
	
		double Middle = (Upper_Bound + Lower_Bound)/2;

		if (Function(Middle)*Function(Upper_Bound) < 0){
			count++; //Increment global count, for output purposes.
			return Bisection(Middle, Upper_Bound, Precision);
		}
		else {
			count++;
			return Bisection(Lower_Bound, Middle, Precision);
		}
	}

}

int main(){

	//Sets precision.
	cout.precision(6);
	cout << left << setw(w) << "Iteration" << setw(w) << "Precision" << setw(w) << "Theta" << endl;
	Bisection(0, PI/2, 0.00001);
	return 0;

}
