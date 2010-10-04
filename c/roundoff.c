/*
	Simple program to show roundoff, underflow
*/
#include<stdio.h>
#include<math.h>
#include<stdbool.h>
#include<stdlib.h>

#define VALUE 300000000

int main () {
	float pop = VALUE;
	printf("increment float %.1f\n", pop);
	pop++;
	printf("%.1f\n", pop); // prints VALUE

	double dpop = VALUE;
	printf("increment double %.1lf\n", dpop);
	dpop++;
	printf("%.1lf\n", dpop); // prints VALUE + 1

	pop = 0.0;
	printf("sum float from %.1f to %d\n", pop, VALUE);
	for (int i = 0; i < VALUE; ++i)
		pop++;
	printf("%.1f\n", pop); // prints 16777216.0

	dpop = 0.0;
	printf("sum double from %.1lf to %d\n", dpop, VALUE);
	for (int i = 0; i < VALUE; ++i)
		dpop++;
	printf("%.1lf\n", dpop); // prints VALUE

	unsigned long n = 1;
	float last=-1, total=0;
	bool done = false;
	printf("float sum(1/n) as n from 1 -> inf\n");
	while (!done) {
		total += 1/(float)n;

		if ((n%100000000) == 0)
			printf("n:%ld, total:%f\n", n, total);

		if (total - last == 0)
			done = true;
		else
			last = total;
		n++;
	}
	printf("successive itereations identical at n=%ld, sum=%f\n", n-1, total);
	
	n = 1;
	double dlast=-1, dtotal=0;
	done = false;
	printf("double sum(1/n) as n from 1 -> inf\n");
	while (!done) {
		dtotal += 1/(double)n;

		if ((n%100000000) == 0)
			printf("n:%ld, dtotal:%lf\n", n, dtotal);

		if ((dtotal - dlast) == 0)
			done = true;
		else
			dlast = dtotal;
		n++;
	}
	printf("successive itereations identical at n=%ld, sum=%lf\n", n-1, dtotal);
}


