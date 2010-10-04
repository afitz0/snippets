#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

const long SIZE = 100000000;

void CountingSort(short* a, long a_size) {
	if ( a_size <  2 )	return;

	const long numCounts = SHRT_MAX;
	long i, j, *count;

	// one count for each possible value of an 8-bit element (0-255)
	count = (long*)malloc(sizeof(long)*numCounts);
	#pragma omp parallel for
	for (i = 0; i < numCounts; i++)
		count[i] = 0;

	// Scan the array and count the number of times each value appears
	for (i = 0; i < a_size; i++) {
		count[ a[i] ]++;
	}

	// Fill the array with the number of 0's that were counted, followed by the number of 1's, and then 2's and so on
	long n = 0;
	for (i = 0; i < numCounts; i++)
		for (j = 0; j < count[i]; j++)
			a[ n++ ] = (short)i;
}

double diff_time(struct timeval *st, struct timeval *et) {
	return (((double)et->tv_sec + ((double)et->tv_usec / 1000000))
		- ((double)st->tv_sec + ((double)st->tv_usec / 1000000)));
}

int main () {
	int i;
	short *a;
	struct timeval st, et;
	double runtime;

	srandom(time(NULL));

	a = (short*)malloc(sizeof(short)*SIZE);

	for (i = 0; i < SIZE; i++)
		a[i] = random() % SHRT_MAX;

	gettimeofday(&st, NULL);
	CountingSort(a, SIZE);
	gettimeofday(&et, NULL);

	runtime = diff_time(&st, &et);

	printf("Runtime: %lf\n", runtime);

	free(a);
}
