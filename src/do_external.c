#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int do_external(char **argv) {
	int child_pid = 0;
	int stat_loc = 0;
	int i = 0;
	int error;
//
//	puts("---------");
//	for (i = 0; argv[i] != NULL; i++)
//		printf("%d %s\n", i, argv[i]);
//	puts("---------");

	if ((child_pid = fork()) != 0) {
		// parent process
		waitpid(child_pid, &stat_loc, 0);
	}
	else {
		// child process
		error = execve(argv[0], argv, 0);
        if (error == -1) {
            
        }
	}

	return 0;
}