/* Intentionally buggy MPI program
 *
 * Aaron Weeden, May/2010
 */
#include <stdio.h>  /* printf */
#include <mpi.h>    /* MPI calls */

/* One process sends a message, another receives it */
int main(int argc, char** argv)
{
	int my_rank, message, source, tag, destination;
	
	MPI_Status status;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if(my_rank == 0)
	{
		printf("Rank %d is sending\n",my_rank);
		MPI_Send(&message,1,MPI_INT,destination,tag,MPI_COMM_WORLD);
		printf("Rank %d sent\n",my_rank);
	}
	else
	{
		printf("Rank %d is receiving\n",my_rank);
		MPI_Recv(&message,1,MPI_INT,source,tag,MPI_COMM_WORLD,&status);
		printf("Rank %d received\n",my_rank);
	}

	MPI_Finalize();
}
