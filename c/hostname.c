/*
	Get hostname by address.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

const int NAME_LEN = 64;

int main () {
	char *name = (char*)malloc(sizeof(char)*NAME_LEN);
	char *address = (char*)malloc(sizeof(char)*NAME_LEN);
	struct hostent * host;

	struct sockaddr_in addr;

	gethostname(name, NAME_LEN);
	printf("name: %s\n", name);
	host = gethostbyname(name);
	inet_ntop(AF_INET, host->h_addr, address, NAME_LEN);
	printf("h_addr: %s\n", address);
	inet_pton(AF_INET, address, &addr.sin_addr.s_addr);

	return EXIT_SUCCESS;
}
