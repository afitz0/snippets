/*
	Testing pointer arithmetic...
*/
#include <stdio.h>
#include <stdlib.h>

int main () {
	int * s = (int*)malloc(sizeof(int));
	int * t = (int*)malloc(sizeof(int));

	*s = 1;
	*t = 2;
	printf("s: %d; t = %d\n", s,t);
	printf("*s: %d; *t = %d\n", *s, *t);

	*s = *t;
	printf("s: %d; t = %d\n", s,t);
	printf("*s: %d; *t = %d\n", *s, *t);

	*s = 1;
	*t = 2;
	printf("s: %d; t = %d\n", s,t);
	printf("*s: %d; *t = %d\n", *s, *t);

	*s = *t++;
	printf("s: %d; t = %d\n", s,t);
	printf("*s: %d; *t = %d\n", *s, *t);

	*s = 1;
	*t = 2;
	printf("s: %d; t = %d\n", s,t);
	printf("*s: %d; *t = %d\n", *s, *t);

	*s++ = *t++;
	printf("s: %d; t = %d\n", s,t);
	printf("*s: %d; *t = %d\n", *s, *t);

	return EXIT_SUCCESS;
}
