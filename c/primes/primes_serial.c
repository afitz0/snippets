#include <stdio.h>
#include <math.h>

const long UPPER = 10000;

int is_prime(long n) {
	int i;

	if (n == 2)
		return 1;

	for (i = 2; i <= sqrt(n); i++) {
		if (n%i == 0)
			return 0;
	}

	return 1;
}

int main(int argc, char ** argv) {
	int i, primes=0;

	for (i = 2; i <= UPPER; i++) {
		primes += is_prime(i);
	}

	printf("Primes between 1 and %ld: %d\n", UPPER, primes);
	// E.g., Primes between 1 and 10000000: 665025
}
