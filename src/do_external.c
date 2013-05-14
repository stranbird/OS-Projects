#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>

int do_external(char **argv) {
    return execve(argv[0], argv, 0);
}