#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

int main (int argc, char ** argv) {
	int rank, size;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int j;
	for (j = 0; j < 100000; ++j) {
		if (rank == 0) {
			int i;
			for (i = 1; i < size; ++i) {
				int buf;
				MPI_Recv(&buf, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
				printf("Rank %d says hi, on iteration %d!.\n", buf, j);
			}
		} else {
			MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
}
