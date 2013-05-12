#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>

int do_external(char **argv) {
	int stat_loc = 0;
	int i = 0;
//
	puts("---------");
	for (i = 0; argv[i] != NULL; i++)
		printf("%d %s\n", i, argv[i]);
	puts("---------");

	if (fork() != 0) {
		// parent process
		waitpid(-1, &stat_loc, 0);
	}
	else {
		// child process
	    if (execve(argv[0], argv, 0) == -1) {
	        puts("execution error");
	    }
	}

	return 0;
}