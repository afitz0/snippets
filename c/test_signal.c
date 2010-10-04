#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sig_handler(int sig) {
	printf("Caught sig %d\n", sig);
}

int main () {
	signal(SIGSTOP, sig_handler);
	pause();
}
