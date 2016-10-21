#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

/**
 * This template function is the recursive function that calculates phi^N.
 * A template function in order to allow us to specify whether we want to
 * call the function using floats or doubles. Although this is slightly
 * messy, because it is possible to call the function with any other
 * type and there are no checks on the type, it has been decided that
 * because the function is only used internally that the risk is negligible.
 *
 * PRECONDITION: typename precision is a floating-point type.
 *
 * returns: phi^N
 */
template<typename precision>
precision recursion_relation(int N, precision Phi_Zero, precision Phi_One)
{
	if (N == 0){ //Base case
		return Phi_Zero;
	}
	else if (N == 1){ //Base case
		return Phi_One;
	}
	else { //Call function with N-2 and N-1, as per recursion relation.
		return recursion_relation<precision>(N-2, Phi_Zero, Phi_One)
			- recursion_relation<precision>(N-1, Phi_Zero, Phi_One);
	}
}

/**
 * Main method opens a file called "output" and prints the outputs to it in a
 * tabular format.
 */
int main(){

	ofstream output_file;
	output_file.open("output");
	output_file.precision(15); //Set precision for floating-point output.

	double phi = (-1+sqrt(5))/2;
	double pow_phi = phi; //Initialise our power of phi to phi, ie. phi^1.
	int N;
	cout << "Please enter N (recommended not to enter N > ~40): ";
	cin >> N;
	cout << endl << "Writing to file 'output'..." << endl;

	//Table heading for basic multiplying calculation.
	output_file << endl << "Calculated directly:" << endl;
	output_file << "n\tphi^n" << endl;
	for (int i = 0; i < N; i++){
		output_file << i+1 << "\t" << pow_phi << endl;		
		pow_phi *= phi;
	}

	cout << "Done direct multiplication..." << endl;	

	//Heading for recursion using floats.
	output_file << endl << "Recursion with floats:" << endl;
	output_file << "n\tphi^n" << endl;
	for (int j = 0; j < N; j++){
		output_file << j+1 << "\t" << recursion_relation<float>(j+1, 1, float(phi)) << endl;
	}

	cout << "Done float recursion..." << endl;

	//Heading for recursion using doubles.
	output_file << endl << "Recursion with double:" << endl;
	output_file << "n\tphi^n" << endl;
	for (int k = 0; k < N; k++){
		output_file << k+1 << "\t" << recursion_relation<double>(k+1, 1, phi) << endl;
	}

	cout << "Done double recursion... Finished!" << endl;

	output_file.close(); //Closing output file.

}
