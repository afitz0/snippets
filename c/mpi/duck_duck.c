/*

Simple MPI instructional program based on the activity outlined here:
  http://wiki.sc-education.org/index.php/MPI_Duck,_Duck..._Goose!

*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

enum {
	SERVER = 0,
	CLOCKWISE,
	COUNTERWISE
};

int main(int argc, char ** argv) {
	int rank, size;
	int target;
	int direction;
	int dest;
	bool done = false;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	srandom(time(NULL));

	// Choose the rank of the first target and send it next
	if (rank == SERVER) {
		target = random() % size;
		printf("Rank %d: I'm going to goose rank %d.\n", rank, target);
		MPI_Send(&target, 1, MPI_INT, (rank + 1) % size, CLOCKWISE, MPI_COMM_WORLD);
	}

	while(!done) {
		MPI_Recv(&target, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		// Regardless of what the target is, we need to find/set the direction
		direction = (status.MPI_TAG == COUNTERWISE) ? COUNTERWISE : CLOCKWISE;
		if (direction == CLOCKWISE)
			dest = (rank + 1) % size;
		else {
			if (rank == 0)
				dest = size - 1;
			else
				dest = (rank - 1) % size;
		}

		// If I'm the target, choose another target, switch directions, and continue
		if (rank == target) {
			direction = (direction == COUNTERWISE) ? CLOCKWISE : COUNTERWISE;

			target = random() % size;
			if (direction == COUNTERWISE) {
				if (rank == 0)
					dest = size - 1;
				else
					dest = (rank - 1) % size;
			} else
				dest = (rank + 1) % size;

			printf("Rank %d: I was goosed and now I'm going to get rank %d.\n", rank, target);
			MPI_Send(&target, 1, MPI_INT, dest, direction, MPI_COMM_WORLD);
		} else {
			printf("Rank %d: Passing along a message from rank %d...\n", rank, status.MPI_SOURCE);
			MPI_Send(&target, 1, MPI_INT, dest, direction, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
}
