#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>

double Derivative(double x, double y);

double Analytic(double x);

double Euler(double (*d)(double, double), double startY, double startX, int intervals, double finalX);

int main()
{
	double startY, startX, finalX;

	startY = 0;
	startX = 0;
	finalX = M_PI/4;	
	
	int intervals;
	printf("Please input no. of intervals: ");
	std::cin >> intervals;

	FILE * file;

	file = fopen("euler_out", "w");

	printf("Writing to file 'euler_out'...\n");
	
	double answer;
	double actual = Analytic(finalX);

	fprintf(file, "%-10s%-20s%-20s\n", "Intervals", "Result", "Analytic Error");
	
	for (int i = 1; i <= intervals; i++)
	{
		answer = Euler(Derivative, startY, startX, i, finalX);
		fprintf(file, "%-10i%-20.15f%-20.15f\n", i, answer, std::abs((answer-actual)/actual));
	}

	fclose(file);

	printf("Done!\n");

	return 0;
}

double Derivative(double x, double y)
{
	return 1 + y*y;
}

double Analytic(double x)
{
	return std::tan(x);
}

double Euler(double (*d)(double, double), double startY, double startX, int intervals, double finalX)
{
	double h = (finalX-startX)/intervals;

	double y0, y1, x;

	y0 = startY;
	x = startX;

	for (int i = 0; i != intervals; i++)
	{
		y1 = y0 + h*(*d)(x, y0);

		y0 = y1;
		x = x + h;
	}

	return y0;
}
