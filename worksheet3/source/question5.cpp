#include <cstdio>
#include <cmath>
#include <iostream>

struct Vector
{
	double one;
	double two;

	Vector operator+(Vector const& other)
	{
		Vector temp;
		temp.one = one + other.one;
		temp.two = two + other.two;
		return temp;
	}
	Vector operator-(Vector const& other)
	{
		Vector temp;
		temp.one = one - other.one;
		temp.two = two - other.two;
		return temp;
	}

};

Vector operator*(Vector const& lhs, double rhs)
{ 
	Vector temp;
	temp.one = lhs.one * rhs;
	temp.two = lhs.two * rhs;
	return temp;
}
Vector operator*(double lhs, Vector const& rhs)
{ 
	Vector temp;
	temp.one = lhs * rhs.one;
	temp.two = lhs * rhs.two;
	return temp;
}
Vector operator/(Vector const& lhs, double rhs)
{ 
	Vector temp;
	temp.one = lhs.one / rhs;
	temp.two = lhs.two / rhs;
	return temp;
}

Vector Derivative(double t, Vector y);

Vector Analytic(double t);

Vector RungeKuttaSecond(Vector (*d)(double, Vector), Vector startY, double startT, int intervals, double finalT);

int main()
{
	Vector startY;
	startY.one = 1;
	startY.two = 0;
	double startT = 0;
	
	double finalT;
	printf("Please input value of t to calculate: ");
	std::cin >> finalT;
	
	int intervals;
	printf("Please input no. of intervals: ");
	std::cin >> intervals;

	FILE * file;

	file = fopen("RK_out", "w");
	
	Vector answer;
	Vector actual = Analytic(finalT);
	
	printf("Writing to file 'RK_out'...\n");

	fprintf(file, "%-10s%-20s%-20s%-20s%-20s\n",
		"Intervals", "Result V", "Result X", "Analytic Error V", "Analytic Error X");
	
	for (int i = 1; i <= intervals; i++)
	{
		answer = RungeKuttaSecond(Derivative, startY, startT, i, finalT);
		fprintf(file, "%-10i%-20.15f%-20.15f%-20.15f%-20.15f\n", 
			i, answer.one, answer.two, 
			std::abs((answer.one-actual.one)/actual.one),
			std::abs((answer.two-actual.two)/actual.two));
	}

	fclose(file);

	printf("Done!\n");

	return 0;
}

Vector Derivative(double t, Vector y)
{
	Vector temp;
	temp.one = -y.two;
	temp.two = y.one;
	return temp;
}

Vector Analytic(double t)
{
	Vector temp;
	temp.one = std::cos(t);
	temp.two = std::sin(t);
	return temp;
}

Vector RungeKuttaSecond(Vector (*d)(double, Vector), Vector startY, double startT, int intervals, double finalT)
{
	double h = (finalT - startT)/intervals;		
	
	Vector k1, k2, y0, y1;
	double t;

	y0 = startY;
	t = startT;

	for (int i = 0; i != intervals; i++)
	{
		k1 = (h * (*d)(t, y0));
		k2 = (h * (*d)(t + h/2, y0 + k1/2.0));
		y1 = y0 + k2;

		y0 = y1;
	}

	return y0;
}
