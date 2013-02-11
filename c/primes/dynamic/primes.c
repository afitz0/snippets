/* 
   Andrew S. Fitz Gibbon
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../check_prime_brute_force.h"
#include "primes.h"

int main (int argc, char** argv) {
  MPI_Status     status;
  MPI_Datatype   ResultsType;
  MPI_Errhandler primes_errors;
  long           range[2], i;
  int            rank, size;
  results_data   results;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Create new type for results struct
  ResultsType = create_results_type(&results);

  // Set Error Handler
  MPI_Comm_create_errhandler(mpi_errors, &primes_errors);
  MPI_Comm_set_errhandler(MPI_COMM_WORLD, primes_errors);

	// Server (master) code
  if (rank == SERVER) {
    int    wu                     = INITIAL_WORK_UNIT_SIZE;
    float  reduce_thresh          = REDUCTION_THRESHOLD;
    float  reduce_amt             = REDUCTION_AMOUNT;
    double avg_time               = 0.0;
    bool   done                   = false;
    long   next_low               = 2;
    long   total_number_of_primes = 0;
    int    work_outstanding       = 0;

    work_unit* completion_times;
    double start_time, end_time;
    int request_flag, result_flag;
    MPI_Status status, request_status, result_status;

    switch(argc) {
      case 4:
        reduce_amt    = atof(argv[3]);
      case 3:
        reduce_thresh = atof(argv[2]);
      case 2:
        wu = atoi(argv[1]);
      default:
        break;
    }

    // Initialize completion time data
    completion_times = (work_unit*)malloc(sizeof(work_unit)*size);  
    if (completion_times == NULL) {
      fprintf(stderr, "Malloc of completion_times failed");
      MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for (i = 0; i < size; i++) {
      completion_times[i].wu_size   = wu;
      completion_times[i].last_time = 0;
    }

    // Total run timer
    start_time = MPI_Wtime();

    while (!done) {
      // check for requests
      MPI_Iprobe(MPI_ANY_SOURCE, REQUEST_TAG, MPI_COMM_WORLD, &request_flag, &request_status);
      // check for results
      MPI_Iprobe(MPI_ANY_SOURCE, RESULTS_TAG, MPI_COMM_WORLD, &result_flag, &result_status);

      // serve work requests
      if (request_flag) {
        serve_request(request_status.MPI_SOURCE, &next_low,
          completion_times[request_status.MPI_SOURCE].wu_size);

        work_outstanding++;
      }

			// retrieve results from worker
			if (result_flag) {
        int worker = result_status.MPI_SOURCE;

        total_number_of_primes +=
          receive_results(ResultsType, worker, completion_times, &avg_time, reduce_thresh, reduce_amt);
        work_outstanding--;

        if (DEBUG) {
          printf("\tRank %d's last runtime: %lf\n",   worker, completion_times[worker].last_time);
          printf("\tThat rank's wu: %d\n",            completion_times[worker].wu_size);
          printf("\tAverage runtime: %lf\n",          avg_time);
          printf("\tCurrent number of primes: %ld\n", total_number_of_primes);
        }

      // No requests or results to process
      if (SERVER_DOES_WORK) {
        long low = next_low;
        long high = low + floor(wu/size);
        next_low = high+1;

        total_number_of_primes += find_primes(low, high).number_of_primes;
      } else if (SERVER_SLEEPS) {
        sleep(avg_time);
      }

      // check if we're done
      if (next_low >= RANGE) {
        long range[2] = { -1, -1 };
        int dest;

        for (dest = 1; dest < size; dest++)
          MPI_Send(range, 2, MPI_INT, dest, STOP_TAG, MPI_COMM_WORLD);

        // flush recv buffers, if any
        while (work_outstanding > 0) {
          MPI_Recv(&results, 1, ResultsType, MPI_ANY_SOURCE, RESULTS_TAG, MPI_COMM_WORLD, &status);
          total_number_of_primes += results.number_of_primes;

          if (DEBUG)
            printf("Got %ld more results from rank %d\n", results.number_of_primes, status.MPI_SOURCE);

          work_outstanding--;
        }

        done = true;
      }
    }

    end_time = MPI_Wtime();

    print_stats(completion_times, size, total_number_of_primes, end_time-start_time);

  // worker code
  } else {
    bool done = false;
    int work_request;

    while (!done) {
      // Ask for work from SERVER & wait for response.
      MPI_Sendrecv(&work_request, 1, MPI_INT, SERVER, REQUEST_TAG, 
                    range, 2, MPI_LONG, SERVER, MPI_ANY_TAG,
                   MPI_COMM_WORLD, &status);

      switch (status.MPI_TAG) {
        // Got work unit
        case WORK_UNIT_TAG:
          if (DEBUG)
            printf("Rank %d received range: %ld - %ld\n", rank, range[0], range[1]);

          results = find_primes(range[0], range[1]);

          if (DEBUG) {
            printf("Rank %d found %ld primes between %ld and %ld\n",
              rank, results.number_of_primes, range[0], range[1]);
          }

          MPI_Send(&results, 1, ResultsType, SERVER, RESULTS_TAG, MPI_COMM_WORLD);
          break;

        // Got stop message, no more work to do
        case STOP_TAG:
          if (DEBUG)
            printf("Rank %d received done message\n", rank);
          done = true;
          break;

      // Got unknown message
        default:
          fprintf(stderr,
                  "Worker %d received an unknown message from the server.\n",
                  rank);
      }
    }
  }

  MPI_Finalize();
}

MPI_Datatype create_results_type(results_data * r) {
  MPI_Aint      disp[2], runtime_addr, number_of_primes_addr, results_addr;
  MPI_Datatype  ResultsType;

  // Types, in order, as defined in results_data
  MPI_Datatype  types[2] = { MPI_DOUBLE, MPI_LONG };

  // How many of each of those types
  int           blocklen[2] = { 1, 1 };

  MPI_Get_address(r, &results_addr);
  MPI_Get_address(&(r->runtime), &runtime_addr);
  MPI_Get_address(&(r->number_of_primes), &number_of_primes_addr);

  disp[0] = runtime_addr - results_addr;
  disp[1] = number_of_primes_addr - results_addr;

  MPI_Type_create_struct(2, blocklen, disp, types, &ResultsType);
  MPI_Type_commit(&ResultsType);

  return ResultsType;
}

void serve_request(int source, long * low, int wu_size) {
  int work_request;
  MPI_Status status;
  long range[2];

  if (DEBUG)
    printf("Probe shows request from %d, receiving...\n", source);

  MPI_Recv(&work_request, 1, MPI_INT, source, REQUEST_TAG, MPI_COMM_WORLD, &status);

  range[0] = (*low);
  range[1] = (*low) + wu_size;

  if (range[1] >= RANGE)
    range[1] = RANGE;
  *low = range[1] + 1;

  if (DEBUG) {
    printf("Sending range %ld - %ld to rank %d\n", range[0], range[1], source);
    fflush(stdout);
  }

  MPI_Send(range, 2, MPI_LONG, source, WORK_UNIT_TAG, MPI_COMM_WORLD);
}

results_data find_primes(long lower, long upper) {
  long i;
  struct timeval start, end;
  results_data results;

  // start timer
  gettimeofday(&start, NULL);

  // Do work
  results.number_of_primes = 0;
  for (i = lower; i <= upper; i++) {
    results.number_of_primes += check_prime_brute_force(i);
  }

  // end timer
  gettimeofday(&end, NULL);

  // calc time differences
  results.runtime = ((double)end.tv_sec + ((double)end.tv_usec/1000000)) - 
    ((double)start.tv_sec + ((double)start.tv_usec/1000000));

  return results;
}

void print_stats(work_unit* data, int procs, long primes, double runtime) {
  int avg_wu, max_wu, min_wu, i;
  double avg_time;

  avg_wu = 0;
  avg_time = 0;
  max_wu = INITIAL_WORK_UNIT_SIZE;
  min_wu = INITIAL_WORK_UNIT_SIZE;
  for (i = 1; i < procs; i++) {
    avg_wu += data[i].wu_size;
    max_wu = (max_wu > data[i].wu_size) ? max_wu : data[i].wu_size;
    min_wu = (min_wu < data[i].wu_size) ? min_wu : data[i].wu_size;
    avg_time += data[i].last_time;
  }
  avg_wu /= procs-1;
  avg_time /= procs-1;

  printf(">\tN       Max prime value\n");
  printf(">\tNp      Number of primes between 2 and N\n");
  printf(">\tAvgT    Average time to complete a work request\n");
  printf(">\tMinN    Minimum size of a work request\n");
  printf(">\tMaxN    Maximum size of a work request\n");
  printf(">\tAvgN    Average size of a work request\n");
  printf(">\tT       Run time\n");
  printf(">\n");
  printf("> N\t\tNp\tAvgT\tMinN\tMaxN\tAvgN\tT\n");
  printf("%ld\t%ld\t%.4lf\t%d\t%d\t%d\t%.4lf\n",
    RANGE, primes, avg_time, min_wu, max_wu, avg_wu, runtime);
}

int receive_results(MPI_Datatype type, int source, work_unit * data,
                   double * avg_time, double thresh, double amt) {
  results_data  results;
  MPI_Status    status;
  double        at = *avg_time;

  MPI_Recv(&results, 1, type, source, RESULTS_TAG, MPI_COMM_WORLD, &status);

  data[status.MPI_SOURCE].last_time = results.runtime;

  if (0 == at)
    at = results.runtime;
  else { // if worker took too long...
    if (results.runtime > at &&
        results.runtime * thresh > results.runtime-at) {
      data[status.MPI_SOURCE].wu_size *= (1-amt);
    }

    at = (at + results.runtime) / 2;
  }

  return results.number_of_primes;
}

void mpi_errors(MPI_Comm *comm, int *err, ...) {
  char err_str[MPI_MAX_ERROR_STRING];
  int len;

  MPI_Error_string(*err, err_str, &len);

  fprintf(stderr, "%s\n", err_str);

  MPI_Abort(MPI_COMM_WORLD, *err)
}
