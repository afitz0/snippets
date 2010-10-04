/* 
	 Andrew Fitz Gibbon
	 Originally written: Oct 2007
	 Rewritten and revised: Sep 2008
	 Reviewed again: May 2010

	 This program was originally developed against LAM/MPI version 7.1.2
	 (http://www.lam-mpi.org)

	 To compile and run:
	 $ mpicc -Wall -lm -o primes_dc primes_dc.c
	 $ mpirun -np 1 ./primes_dc [upper_value]

Overview:
This program counts the number of primes between 1 and DEFAULT_BOUND or the given
upper value.  The implementation is a simple divide and conquer parallel algorithm
whereby each new process is given a range of numbers.  If the size of the range is
greater than the DEFAULT_THRESHOLD, the process will spawn SPLIT_FACTOR - 1 new
processes and then count the number of primes in the top portion.  Otherwise, the 
number of primes in its given range is counted.  When one process obtains a result,
it's sent to its parent who sums all results from itself and its children and passes
it on to its parent.  In this fashion, the root process will receive the final total
and print it to stdout.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "primes_dc.h"

/* 
	 For the initial process (the "root" of the tree):
		 argv[1], if it exists, is the new upper bound
	 For dynamically spawned processes:
		 argv[1] is the size of the range of the numbers
		 argv[2] is the lowest value for the current group of processes
	 The number of primes between 1 and DEFAULT_BOUND is printed to stdout.
 */
int main (int argc, char** argv) {
	struct host_data data;
	double t1, t2, elapsed;
	int ret, i, size, rank, len;
	char comm_name[MPI_MAX_OBJECT_NAME];

	#ifdef DEBUG
	printf("------\n");
	for (i = 0; i < argc; ++i)
		printf("argv[%d]: %s\n", i, argv[i]);
	#endif

	ret = MPI_Init(&argc, &argv);
	if (ret != MPI_SUCCESS)
		mpi_error(ret, __FILE__, __LINE__);

	MPI_Comm_set_errhandler(MPI_COMM_SELF, MPI_ERRORS_RETURN);
	MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

	t1 = MPI_Wtime();

	if (init_data(&data, argc, argv) == -1)
		error_exit("Failed to initialize data.\n", __FILE__, __LINE__);

		if (spawn_children(&data, argv[0]) == -1)
			error_exit("Failed while attampting to spawn children.\n", __FILE__, __LINE__);

#ifdef DEBUG
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(data.parent_comm, &rank);
	printf("[%d] rank %d of %d in MPI_COMM_WORLD\n", getpid(), data.rank, size);
	MPI_Comm_size(data.parent_comm, &size);
	printf("[%d] rank %d or %d in parent_comm\n", getpid(), rank, size);
	printf("Working %d to %d (range_size: %ld).\n",
			data.lower_bound, data.upper_bound, data.range_size);
	fflush(stdout);
#endif

	data.sub_total = calculate_primes(data.lower_bound, data.upper_bound);

#ifdef DEBUG
	printf("sub_total: %d\n", data.sub_total);
	fflush(stdout);
#endif

	if (aggregate_total(&data) == -1)
		error_exit("Failed while trying to aggregate totals.\n", __FILE__, __LINE__);

	if (data.parent_comm == MPI_COMM_NULL) {
		t2 = MPI_Wtime();
		elapsed = t2 - t1;

		printf("Time elapsed: %F seconds\n", elapsed);
		printf("Primes between 1 and %d: %d\n", data.upper_bound, data.total);
		fflush(stdout);
	}

	MPI_Finalize();
	return(0);
}

int init_data(struct host_data *data, int argc, char** argv) {
	int ret;

	ret = MPI_Comm_rank(MPI_COMM_WORLD, &data->rank);
	if (ret != MPI_SUCCESS)
		mpi_error(ret, __FILE__, __LINE__);

	ret = MPI_Comm_get_parent(&data->parent_comm);
	if (ret != MPI_SUCCESS)
		mpi_error(ret, __FILE__, __LINE__);

	if (data->parent_comm == MPI_COMM_NULL) {
		if (argc == 2)
			data->range_size = atoi(argv[1]);
		else
			data->range_size = DEFAULT_BOUND;

		data->lowest_bound = 1;
	} else if (argc != 3) {
		error_exit("A child was started with wrong number of arguments.\n", __FILE__, __LINE__);
	} else {
		data->range_size = atoi(argv[1]);
		data->lowest_bound = atoi(argv[2]);
	}

	if (data->rank == 0)
		data->lower_bound = data->lowest_bound;
	else
		data->lower_bound = data->lowest_bound + (data->rank * data->range_size);
	data->upper_bound = data->lower_bound + (data->range_size - 1);

	data->threshold = DEFAULT_THRESHOLD;
	data->sub_total = 0;
	data->total = 0;
	data->spawned_children = false;

	return(0);
}

int spawn_children(struct host_data *data, char* program_name) {
	int i, len, ret;
	char** child_argv;
	char str[MPI_MAX_ERROR_STRING];
	int err_codes[SPLIT_FACTOR-1];

	if (data->range_size > data->threshold) {
		data->range_size = data->range_size / SPLIT_FACTOR;
		data->spawned_children = true;

		child_argv = (char**)malloc(3*sizeof(char*));

		child_argv[0] = (char*)malloc(64*sizeof(char));
		child_argv[1] = (char*)malloc(64*sizeof(char));
		child_argv[2] = (char*)NULL;

		sprintf(child_argv[0], "%ld", data->range_size);
		sprintf(child_argv[1], "%d", data->lower_bound);

#ifdef DEBUG
		printf("Spawning children.\n");
		fflush(stdout);
#endif

		ret = MPI_Comm_spawn(program_name,
				child_argv,            // Arguments as defined above
				SPLIT_FACTOR - 1,      // children + myself = SPLIT_FACTOR processes working
				MPI_INFO_NULL,         // We're not going to care where MPI puts the children
				0,                     // 0 is the only rank in my own communicator
				MPI_COMM_SELF,         // Will manifest itself as "parent_comm" in the child
				&data->children_comm, err_codes); // Communicator and error codes for the children
		
#ifdef DEBUG
		MPI_Error_string(ret, str, &len);
		printf("%s\n", str);

		for (i = 0; i < SPLIT_FACTOR-1; ++i) {
			MPI_Error_string(err_codes[i], str, &len);
			printf("%s\n", str);
		}
#endif

		data->lower_bound = data->lower_bound + ((SPLIT_FACTOR - 1) * data->range_size);

		free(child_argv[0]);
		free(child_argv[1]);
		free(child_argv);
	}


	return(0);
}

int calculate_primes(int lower, int upper) {
	int i;
	int sub_total = 0;

	if (lower >= upper)
		error_exit("Lower bound for calculation larger than upper bound.\n", __FILE__, __LINE__);

	for (i = lower; i <= upper; i++)
		sub_total += check_prime_brute_force(i);

	return(sub_total);
}

int check_prime_brute_force(unsigned long number) {
	unsigned long upper_bound, remainder, divisor = 3;

	// 2 is the only number not caught below
	if (number == 2)
		return(1);

	remainder = number % 2; 
	if (remainder == 0)
		return(0);

	/* Say number is not prime, then it factors as n = ab.  Consequently 
		 one of {a, b} has to be <= sqrt(n), and the other has to be >= sqrt(n).  
		 That is if both a, b are < sqrt(n) then ab < n.  Thanks Tim McLarnan. */
	upper_bound = sqrt(number); 

	while (divisor <= upper_bound) {
		remainder = number % divisor; 

		if (remainder == 0)
			return(0); 

		/* Since we filter all the even numbers we don't need to divide by
			 even divisors.  If we initialize divisor to 3 we can increment by 2
			 each time and cut our work in half.  Thanks Tim McLarnan.  */
		divisor += 2;           
	}

	return 1;
}

int aggregate_total(struct host_data *data) {
	MPI_Status recv_status;
	int i, ret;

	if (data->parent_comm != MPI_COMM_NULL) {
		data->total += data->sub_total;

		if (data->spawned_children) {
			for (i = 0; i < SPLIT_FACTOR - 1; i++) {
				ret = MPI_Recv(&data->sub_total, 1, MPI_INT,
							i, TAG, data->children_comm, &recv_status);
				if (ret != MPI_SUCCESS)
					mpi_error(ret, __FILE__, __LINE__);

				data->total += data->sub_total;

#ifdef DEBUG
				printf("Received %d, total now %d.\n", data->sub_total, data->total);
				fflush(stdout);
#endif
			}
		}

		ret = MPI_Send(&data->total, 1, MPI_INT,
				0, TAG, data->parent_comm);
		if (ret != MPI_SUCCESS)
			mpi_error(ret, __FILE__, __LINE__);
	} else {
		data->total += data->sub_total;
		if (data->spawned_children) {
			for (i = 0; i < SPLIT_FACTOR - 1; i++) {
				ret = MPI_Recv(&data->sub_total, 1, MPI_INT,
					i, TAG, data->children_comm, &recv_status);
				if (ret != MPI_SUCCESS)
					mpi_error(ret, __FILE__, __LINE__);

				data->total += data->sub_total;
			}
		}
	}

	return(0);
}
