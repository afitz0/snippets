/*
	Randomly generates arithmetic problem
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

const int DEFAULT_PROBS = 100;
const int UPPER_VAL = 100;

int main (int argc, char ** argv) {
	int probs = DEFAULT_PROBS;

	srandom(time(NULL));
	
	if (argc == 2)
		probs = (int)strtol(argv[1], NULL, 10);

	for (int i = 0; i < probs; ++i) {
		int op = random() % 4;
		double a, b, ans, guess;
		char str;

		a = UPPER_VAL * ((double)random() / (double)RAND_MAX);
		b = UPPER_VAL * ((double)random() / (double)RAND_MAX);

		switch (op) {
			case 0:
				ans = a + b;
				str = '+';
				break;
			case 1:
				ans = a * b;
				str = '*';
				break;
			case 2:
				ans = a - b;
				str = '-';
				break;
			case 3:
				ans = a / b;
				str = '/';
				break;
			default:
				break;
		}

		printf("%f %c %f = ", a, str, b);
		fscanf(stdin, "%lf\n", &guess);
		if (guess != ans)
			printf("WRONG. Answer is %f, you said %f\n", ans, guess);
	}
}
