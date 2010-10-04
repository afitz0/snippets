/*
	 Testing throttled/unthrottled MPI_Send/recv
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

int main (int argc, char ** argv) {
	int rank, size;
	MPI_Status status;
	MPI_Request req;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int j;
	for (j = 0; j < 100000; ++j) {
		if (rank == 0) {
			int i;
			for (i = 1; i < size; ++i) {
				int buf = 1;
				if (j%500 == 1) {
					puts("Telling others to slow down.");
					//MPI_Bcast(&buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
					MPI_Send(&buf, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
				}

				MPI_Recv(&buf, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
				printf("Rank %d says hi on iteration %d!.\n", buf, j);
			}
		} else {
			if (j%500 == 1) {
				int flag;
				MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
				//printf("Flag: %s\n", (flag ? "true" : "false"));
				if (flag) {
					printf("Rank %d sleeping for 1 second...\n", rank);
					MPI_Recv(&flag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					sleep(1);
				}
			}

			MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
}
