/*
	Attempt to show off underflow/overflow when using MPI.
*/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<mpi.h>
#include<limits.h>
#include <stdbool.h>

int main (int argc, char** argv) {
	int rank,size;
	long long n;
	float last, total, ltotal;
	double dlast, dtotal, ldtotal;
	bool done;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	ltotal=0;
	last=-1;
	n=1+rank;
	done = false;
	while(!done) {
		ltotal+=1/(float)n;

		if (n%100000000 == 0) {
			printf("n: %lld; ltotal: %f; last: %f\n", n, ltotal, last);
			fflush(stdout);
		}

		if (ltotal-last == 0)
			done = true;
		else
			last=ltotal;

		n+=size;
	}

	printf("total on rank %d: %f\n", rank, ltotal);

	MPI_Reduce(&ltotal, &total, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("convergant at n=%lld(+/-1), total=%f\n", n, total);

	ldtotal=0;
	dlast=-1;
	n=1+rank;
	done = false;
	while(!done) {
		ldtotal+=1/(double)n;

		if (n%100000000 == 0) {
			printf("n: %lld; ldtotal: %lf; dlast: %f\n", n, ldtotal, dlast);
			fflush(stdout);
		}

		if (ldtotal-dlast == 0)
			done = true;
		else
			dlast=ldtotal;

		n+=size;
	}

	printf("total on rank %d: %f\n", rank, ldtotal);

	MPI_Reduce(&ldtotal, &dtotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("convergant at n=%lld, total=%lf\n", n, dtotal);

	MPI_Finalize();
}
