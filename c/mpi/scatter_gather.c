#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNK  50
#define SERVER 0

int main (int argc, char ** argv) {
	int rank, size, i, total_size;
	int *entire_array, *sub_array;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	total_size = size*CHUNK;	
	sub_array = (int*)malloc(CHUNK*sizeof(int));

	// Have the server initialize the array
	// Only the server needs to allocate the memory for the entire_array
	if (rank == SERVER) {
		entire_array = (int*)malloc(total_size*sizeof(int));

		for (i = 0; i < total_size; ++i)
			entire_array[i] = i;

	}

	// Everyone scatters, does the "work" and gathers it up again
	MPI_Scatter(entire_array, CHUNK, MPI_INT, sub_array, CHUNK, MPI_INT, SERVER, MPI_COMM_WORLD);

	for (i = 0; i < CHUNK; ++i)
		sub_array[i] *= 2;

	MPI_Gather(sub_array, CHUNK, MPI_INT, entire_array, CHUNK, MPI_INT, SERVER, MPI_COMM_WORLD);

	// The server prints out the array and frees the memory
	if (rank == SERVER) {
		for (i = 0; i < total_size; ++i)
			printf("entire_array[%d]: %d\n", i, entire_array[i]);

		free(entire_array);
	}

	free(sub_array);

	MPI_Finalize();

	return 0;
}
