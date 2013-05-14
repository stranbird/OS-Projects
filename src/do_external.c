#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

#include "includes/glo.h"
#include "includes/pipe.h"

extern int setup_pipe();

int do_external(char **argv) {
	int stat_loc = 0;
	int i = 0;
    FILE *fout = fopen("test/text.txt", "w");
    pid_t pid;
    
    char buf[BUFSIZ];
//
	puts("---------");
    printf("ofd PIPE_R: %d\n", ofd[PIPE_R]);
    printf("ofd PIPE_W: %d\n", ofd[PIPE_W]);
	for (i = 0; argv[i] != NULL; i++)
		printf("%d %s\n", i, argv[i]);
	puts("---------");
    
    if (is_pipe) {
        pipe(fd);
		printf("[fd PIPE_W]%d\n", fd[PIPE_W]);
		printf("[fd PIPE_R]%d\n", fd[PIPE_R]);
    }

	if ((pid = fork()) != 0) {
		// parent process
        if (is_pipe) {
            ofd[PIPE_R] = fd[PIPE_R];
            ofd[PIPE_W] = fd[PIPE_W];
        }
        else
            ofd[PIPE_R] = -1;
        
        waitpid(-1, &stat_loc, 0);
    }
	else {
		// child process
        
        if (is_pipe) {
            close(fd[PIPE_R]);
            close(PIPE_W);
            assert(dup(fd[PIPE_W]) == PIPE_W);
            close(fd[PIPE_W]);
        }
                
        if (ofd[PIPE_R] != -1) {
            close(ofd[PIPE_W]);
//            close(ofd[PIPE_R]);
            read(ofd[PIPE_R], buf, BUFSIZ);
            puts(buf);
        }
        
	    if (execve(argv[0], argv, 0) == -1) {
	        puts("execution error");
	    }
	}

	return 0;
}