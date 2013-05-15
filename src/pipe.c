#include <unistd.h>

#include "includes/pipe.h"

int stdio_use(int io, int fd) {
	close(io);
    dup(fd);
    close(fd);

    return 0;
}

int stdin_use(int fd) {
	stdio_use(PIPE_R, fd);

	return 0;
}

int stdout_use(int fd) {
	stdio_use(PIPE_W, fd);

	return 0;
}

int init_pipe() {
	o_stdin = dup(PIPE_R);
    o_stdout = dup(PIPE_W);

    ofd[PIPE_R] = PIPE_R;
    ofd[PIPE_W] = PIPE_W;

    return 0;
}

int reset_pipe(int *fd) {
    fd[PIPE_R] = PIPE_R;
    fd[PIPE_W] = PIPE_W;
    
    close(PIPE_R);
    dup(o_stdin);
    
    close(PIPE_W);
    dup(o_stdout);

    return 0;
}