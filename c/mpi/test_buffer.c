/*
	Code to (in essence) test the internal buffer limits on MPI.
	Ranks 1..n send as fast as possible, while rank 0 receives them all
	as fast as possible.

	Expected behavior is that rank 0
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

#define DATA 100000

int main (int argc, char ** argv) {
	int rank, size, i, j;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	for (j = 0; j < 100000; ++j) {
		if (rank == 0) {
			for (i = 1; i < size; ++i) {
        int buf;

        printf("Waiting for rank %d on %d\n", i, j);
				MPI_Recv(&buf, 1, MPI_INT, i, j, MPI_COMM_WORLD, &status);
				printf("Rank %d says hi, on iteration %d!\n", status.MPI_SOURCE, j);
			}
		} else {
      MPI_Request r;
			MPI_Isend(&rank, 1, MPI_INT, 0, j, MPI_COMM_WORLD, &r);
		}
	}

	MPI_Finalize();
}
