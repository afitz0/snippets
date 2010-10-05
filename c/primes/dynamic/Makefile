CC = mpicc
CFLAGS = -pedantic -Wall -lm --std=gnu99 -O3 -pg

all: primes

primes: primes.c
	$(CC) $(CFLAGS) -o primes primes.c ../check_prime_brute_force.c

clean:
	rm -f primes
