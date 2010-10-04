// compile with a command like "gcc integrate.c"
// run with the command "./a.out 1000 0 1"
//     to integrate f(x)=x^2 using 1000 steps from 0 to 1
//     the theoretical answer is 1/3
//
// This simple program is very useful in teaching how to do parallel
// programming via MPI and/or OpenMP
#include <stdio.h>
#include <stdlib.h>

// This is the function "f" being integrated
double f(double x) {return x*x;}

// Function "init" calculates deltaX and initializes yVals
void init(int numSeg, double startX, double endX, double *deltaX, double yVals[]) {
	int i;
	double x, rangeX = endX - startX;
	*deltaX = rangeX/numSeg;
	for(i=0; i <= numSeg; ++i) {
		x = startX + i*rangeX/numSeg;
		yVals[i] = f(x);
	}
}

// Add the area of all the trapazoids together, to estimate the integral
double integrate(double yVals[], double numSeg, double deltaX) {
	int i;
	double area=0;
	for (i=0; i<numSeg; ++i)
		area += (yVals[i] + yVals[i+1])*.5*deltaX;
	return area;
}

int main(int argc, char **argv) {
	int numSeg;
	double startX, endX, deltaX, *yVals;

	// Get number of segments, starting and ending values for integral
	if (argc < 4) exit(0);
	numSeg =	atoi(argv[1]);
	startX =	atof(argv[2]);
	endX =		atof(argv[3]);
	if ((numSeg<1) || (startX >= endX)) exit(0);

	// Allocate space for function values and calculate them
	yVals = (double *)malloc(sizeof(double)*(numSeg+1));
	init(numSeg, startX, endX, &deltaX, yVals);

	// Display result
	printf("Area under curve y=x^2 from %g to %g is %.18G\n",
		startX, endX,  integrate(yVals, numSeg, deltaX));
	return 0;
}
