#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>

int do_external(char **argv) {
    char *envp[1];
    envp[0] = 0;

    return execve(argv[0], argv, envp);
}
