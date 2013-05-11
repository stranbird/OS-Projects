#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int do_external(char **argv) {
	int child_pid = 0;
	int stat_loc = 0;

	if ((child_pid = fork()) != 0) {
		// parent process
		waitpid(child_pid, &stat_loc, 0);
	}
	else {
		// child process
		execve(argv[0], argv, 0);
	}

	return 0;
}