/*
	A simple *nix shell. Originally written in 2008 by Andrew Fitz Gibbon
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#ifdef __APPLE__ || __FreeBSD__
// Mac or FreeBSD
#elif __linux__
// Linux
#else
// unknown
#endif

const char * PROMPT     = "$ ";
const char * EXIT       = "exit";
const char * PATH       = "PATH";
const int    MAX_LEN    = 1024;
const int    MAX_ITEM   = 64;
const char * WHITESPACE = " \t";

void     build_argv (char * args, char * argv[MAX_ITEM]);
char *      in_path (char * cmd);
void   print_status (int status);
void             cd (char * dir);

int main (int argc, char ** argv) {
	char * cmd_line = (char*)malloc(MAX_LEN*sizeof(char));
	char * child_argv[MAX_ITEM];
	int i;
	bool done = false;

	do {
		printf(PROMPT);
		if (fgets(cmd_line, MAX_LEN, stdin) != NULL) {
			// remove the trailing newline
			char * n = strchr(cmd_line, '\n');
			if (n != NULL) *n = '\0';

			// if cmd_line is blank, do nothing
			// strcmp's return can be confusing when treated as a boolean,
			// so we're giving it a name.
			bool is_blank = strcmp(cmd_line, "\0");

			if (is_blank) {
				build_argv(cmd_line, child_argv);

				if (!strcmp(child_argv[0], EXIT))
					done = true;
				else if (!strcmp(child_argv[0], "pwd"))
					printf("%s\n", getenv("PWD"));
				else if (!strcmp(child_argv[0], "cd"))
					cd(child_argv[1]);
				else {
					char * cmd = (char*)malloc(MAX_LEN*sizeof(char));

					// Check for absolute vs. in path
					if (child_argv[0][0] == '/' || child_argv[0][0] == '.')
						strcpy(cmd, child_argv[0]);
					else
						cmd = in_path(child_argv[0]);

					if (cmd != NULL) {
						pid_t pid = fork();
						if (pid == 0) {
							execv(cmd, child_argv);
							exit(EXIT_FAILURE);
						}
						
						// Test for background character, '&'.
						bool do_wait = true;
						for (i = 1; child_argv[i] != NULL; ++i)
							do_wait = strcmp(child_argv[i], "&");

						if (do_wait) {
							int s;
							wait(&s);
							print_status(s);
						} else
							printf("[%d] Backgrounded.\n", pid);
					} else
						fprintf(stderr, "Command %s not found.\n", child_argv[0]);
					free(cmd);
				}
			}
		} else {
			if (feof(stdin))
				done = true;
		}
	} while (!done);

	puts("Exiting");

	return EXIT_SUCCESS;
}

void build_argv(char * args, char * argv[MAX_ITEM]) {
	char ** ap;

	// Shamelessly copied from EXAMPLES in BSD's strsep(3)
  for (ap = argv; (*ap = strsep(&args, WHITESPACE)) != NULL;)
    if (**ap != '\0')
      if (++ap >= &argv[MAX_ITEM])
        break;
}

char * in_path(char * cmd) {
	char * path = (char*)malloc(MAX_LEN*sizeof(char));
	strcpy(path, getenv(PATH));
	
	char **ap, *dirs[MAX_ITEM]; 

	// Shamelessly copied from EXAMPLES in BSD's strsep(3)
  for (ap = dirs; (*ap = strsep(&path, ":")) != NULL;)
    if (**ap != '\0')
      if (++ap >= &dirs[MAX_ITEM])
        break;

	int i;
	char * exe = (char*)malloc(MAX_LEN*sizeof(char));
	for (i = 0; dirs[i] != NULL; ++i) {
		sprintf(exe, "%s/%s", dirs[i], cmd);
		if (access(exe, X_OK) != -1)
			return exe;
	}

	free(exe);
	return NULL;
}

void print_status(int status) {
	if (WIFEXITED(status))
		puts("Process exited normally");
	else if (WIFSIGNALED(status)) {
		printf("Process terminated with signal %d\n", WTERMSIG(status));
		if (WCOREDUMP(status))
			puts("Core dumped");
	} else if (WIFSTOPPED(status))
		printf("Process stopeed with signal %d\n", WSTOPSIG(status));
	else
		puts("Process exited with unknown status");
}

void cd(char * dir) {
	char * newdir = (char*)malloc(MAX_LEN*sizeof(char));

	if (dir == NULL)                             // Home directory
		newdir = getenv("HOME");
	else if (dir[0] == '/')                      // Absolute directory
		strcpy(newdir, dir);
	else if (dir[0] == '.' && dir[1] == '.' &&
					(dir[2] == '\0' || dir[2] == '/')) { // Parent directory
		char * pwd = (char*)malloc(sizeof(char)*sizeof(getenv("PWD")));
		pwd = getenv("PWD");

		// Find the last / and replace with null
		char * c = strrchr(pwd, '/');
		if (c != NULL) *c = '\0';

		// If we were in /, then just stay there
		if (pwd[0] == '\0') strcpy(pwd, "/");

		strcpy(newdir, pwd);
	} else if (isalpha(dir[0])) {
		char * d = (char*)malloc(sizeof(char)*(strlen(getenv("PWD"))+strlen(dir)));
		strcat(d, getenv("PWD"));
		strcat(d, "/");
		strcat(d, dir);
		strcpy(newdir, d);
	} else
		puts("No cd case.");

	if (access(newdir, X_OK) != -1) {
		chdir(newdir);
		setenv("PWD", newdir, 1);
	} else
		printf("Could not cd to %s\n", newdir);
}
