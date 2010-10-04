#include<stdlib.h>
#include<stdio.h>
#include<mpi.h>
#include<math.h>
#include"check_prime_brute_force.c"

#define LIMIT						10000000
#define DEFAULT_CHUNK_SIZE		100
#define REQUEST_TAG				21
#define RESULT_TAG 				42

int main (int argc, char ** argv) {
	int i;
	int rank;
	int size;
	int source;
	int chunk_size;
	int request_count = 0;
	int result_count = 0;
	int bounds[2];
	int current_low = 1;
	int rcvd_request;
	int rcvd_result;
	int source_result;
	int total_primes = 0;
	int lower_bound;
	int upper_bound;
	int sub_total = 0;

	double t1, t2, elapsed_time;

	if ( argc == 2 ) {
		chunk_size = atoi( argv[1] );
	} else {
		chunk_size = DEFAULT_CHUNK_SIZE;
	}	

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Master code
	if (rank == 0) {
		t1 = MPI_Wtime();

		while (1) {
			for ( source = 1; source < size; source++ ) {
				MPI_Iprobe(source,
							  REQUEST_TAG,
							  MPI_COMM_WORLD,
							  &rcvd_request,
							  MPI_STATUS_IGNORE);
	
				MPI_Iprobe(source,
							  RESULT_TAG,
							  MPI_COMM_WORLD,
							  &rcvd_result,
							  MPI_STATUS_IGNORE);
		
				if ( rcvd_request ) {
					request_count++;
		
					MPI_Recv(&source,
								1,
								MPI_INT,
								source,
								REQUEST_TAG,
								MPI_COMM_WORLD,
								MPI_STATUS_IGNORE);
		
					if ( current_low > LIMIT ) {
						bounds[0] = -1;
					} else {
						bounds[0] = current_low;
					}
		
					if ( (current_low + chunk_size-1) > LIMIT ) {
						bounds[1] = LIMIT;
					} else {
						bounds[1] = current_low + chunk_size-1;
					}
					current_low += chunk_size;
		
					MPI_Send(bounds,
								2,
								MPI_INT,
								source,
								REQUEST_TAG,
								MPI_COMM_WORLD);
				}
		
				if ( rcvd_result ) {
					result_count++;
		
					MPI_Recv(&source_result,
								1,
								MPI_INT,
								source,
								RESULT_TAG,
								MPI_COMM_WORLD,
								MPI_STATUS_IGNORE);
		
					total_primes += source_result;
				}
			}

			if ( result_count >= (LIMIT / chunk_size) ) {
				for ( source = 1; source < size; source++) {
					bounds[0] = -1;
	
					MPI_Send(bounds,
								2,
								MPI_INT,
								source,
								REQUEST_TAG,
								MPI_COMM_WORLD);
				}
		
				printf("Primes between 1 and %d: %d\n", LIMIT, total_primes);
				break;
			}
		}

		t2 = MPI_Wtime();
		elapsed_time = t2-t1;

		printf("Time: %f\n", elapsed_time);
	}

	// Worker code
	else {
		while (1) {
			sub_total = 0;

			MPI_Send(&rank,
						1,
						MPI_INT,
						0,
						REQUEST_TAG,	
						MPI_COMM_WORLD);
	
			MPI_Recv(bounds,
						2,
						MPI_INT,
						0,
						REQUEST_TAG,
						MPI_COMM_WORLD,
						MPI_STATUS_IGNORE);
	
			lower_bound = bounds[0];
			upper_bound = bounds[1];
		
			if (lower_bound != -1) {
				for (i = lower_bound; i <= upper_bound; i++) {
					if ( check_prime_brute_force(i) ) {
						sub_total++;
					}
				}

				MPI_Send(&sub_total,
							1,
							MPI_INT,
							0,
							RESULT_TAG,
							MPI_COMM_WORLD);
			} else {
				break;
			}
		}
	}

	MPI_Finalize();
	return(0);
}
