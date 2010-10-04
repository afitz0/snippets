const int  SPLIT_FACTOR      = 25;
const int DEFAULT_BOUND     = 10000000;
const int  DEFAULT_THRESHOLD = 100000;
const int  TAG               = 999;

struct host_data {
	int      threshold;
	int      rank;
	int      range_size;
	int      lowest_bound;
	int      lower_bound;
	int      upper_bound;
	int      total;
	int      sub_total;
	bool     spawned_children;
	MPI_Comm parent_comm;
	MPI_Comm children_comm;
};

/* 
	init_data():
		Sets initial values for the host_data struct
*/
int init_data(struct host_data *data, int argc, char** argv);

/*
	spawn_children():
		If data->range_size is greater than data->threshold, spawn SPLIT_FACTOR - 1 
		new processes.  Otherwise spawn_children() has no effect.
 */
int spawn_children(struct host_data *data, char* program_name);

/* 
	 calculate_primes():
		 Returns the number of primes between lower and upper, inclusive
 */
int calculate_primes(int lower, int upper);

/*
	 check_prime_brute_force():
		 Returns 1 if number is prime, otherwise returns 0
 */
int check_prime_brute_force(unsigned long number);

/* 
	 aggregate_total():
		 If the current process has children, then their totals are collected.
		 If the current process is not the root, the total is sent to the parent
 */
int aggregate_total(struct host_data *data);

void mpi_error(int code, char * file, int line) {
	int init, len;
	char str[MPI_MAX_ERROR_STRING];
	char message[MPI_MAX_ERROR_STRING];

	MPI_Initialized(&init);

	if (init)
		MPI_Error_string(code, str, &len);
	else
		sprintf(str, "Unknown error");

	fprintf(stderr, "Fatal error at %s:%d -- %s\n", file, line, str);
	fprintf(stderr, "Terminating process.\n");
	fflush(stderr);

	if (init)
		MPI_Abort(MPI_COMM_WORLD, code);
	exit(EXIT_FAILURE);
}

void error_exit(char * message, char * file, int line) {
	fprintf(stderr, "Fatal error at %s:%d -- %s\n", file, line, message);
	fprintf(stderr, "Terminating process.\n");
	fflush(stderr);

	exit(EXIT_FAILURE);
}
