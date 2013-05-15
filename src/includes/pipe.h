#define PIPE_W 1
#define PIPE_R 0

int o_stdin, o_stdout;
int fd[2], ofd[2];

int stdin_use(int fd);
int stdout_use(int fd);
int stdio_use(int io, int fd);

int init_pipe();
int reset_pipe(int *fd);
