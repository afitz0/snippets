const bool  debug = true;

const bool  SERVER_SLEEPS          = false;
const bool  SERVER_DOES_WORK       = false;
const int   SERVER                 = 0;
const int   INITIAL_WORK_UNIT_SIZE = 10000;
const long  RANGE                  = 100000000;
const float REDUCTION_THRESHOLD    = 0.1; // How much slower than average before reducing WU
const float REDUCTION_AMOUNT       = 0.05; // How much to reduce WU

enum {
 REQUEST_TAG,
 WORK_UNIT_TAG,
 RESULTS_TAG,
 STOP_TAG
};

typedef struct {
  double last_time;
  int    wu_size;
} work_unit;

typedef struct {
  double runtime;
  long   number_of_primes;
} results_data;

MPI_Datatype create_results_type (results_data * r);
void                  mpi_errors (MPI_Comm *comm, int *err, ...);
results_data         find_primes (long lower, long upper);
void                 print_stats (work_unit* data, int procs, long primes, double runtime);
void               serve_request (int source, long * low, int wu_size);
int              receive_results (MPI_Datatype type, int source, work_unit * data,
                                  double * avg_time, double thresh, double amt);
