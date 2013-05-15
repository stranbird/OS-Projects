#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "parser.h"

#include "includes/pipe.h"

#include "includes/glo.h"

#include "includes/builtin.h"

#include "includes/do_external.h"

#define try_redirect(io, fd) \
    if (fd[io] != io) { \
        stdio_use(io, fd[io]); \
        fd[io] = io; \
    } \

void type_prompt(int flag) {
    if (flag == NORMAL) {
        printf("%% ");
    }
}

bool is_internal(const char *cmd) {
    char *internal_cmd[] = {
        "cd",
        "pwd",
        "ls"
    };
    int i;
    
    for (i = 0; i != sizeof(internal_cmd) / sizeof(char *); i++)
        if (strcmp(cmd, internal_cmd[i]) == 0)
            return YES;
 
    return NO;
}

bool is_external(const char *cmd) {
    return access(cmd, X_OK) == 0;
}

bool is_cmd(const char *a_cmd, const char *b_cmd) {
    return strcmp(a_cmd, b_cmd) == 0;
}

int main()
{
    char **argv;
    bool flag = NORMAL;
    int stat;
    pid_t pid;
    
    init_pipe();
    
    while (YES) {
        type_prompt(flag);        
        reset_pipe(fd);
        getcmd(&argv, &flag, &(fd[PIPE_R]), &(fd[PIPE_W]));
        
        if (flag == TO_PIPE) {
            pipe(fd);
        }
        else if (flag == NORMAL) {
            try_redirect(PIPE_R, fd);
            try_redirect(PIPE_W, fd);
        }
        
        if (argv[0] == NULL)
            continue;
        if (strcmp(argv[0], "exit") == 0)
            break;
        else if (is_internal(argv[0])) {

            if (is_cmd(argv[0], "pwd"))
                do_pwd(argv);
            else if (is_cmd(argv[0], "cd"))
                do_cd(argv);
            else if (is_cmd(argv[0], "ls"))
                do_ls(argv);

        }
        else if (is_external(argv[0])) {
            if ((pid = fork()) != 0) {
                if (flag == TO_PIPE) {
                    close(fd[PIPE_W]);
                    ofd[PIPE_R] = fd[PIPE_R];
                }
                else
                    ofd[PIPE_R] = PIPE_R;
                
                waitpid(pid, &stat, 0);
            }
            else {
                if (fd[PIPE_W] != PIPE_W) {
                    if (fd[PIPE_R] != PIPE_R)
                        close(fd[PIPE_R]);
                    stdout_use(fd[PIPE_W]);
                }
                
                if (ofd[PIPE_R] != PIPE_R) {
                    stdin_use(ofd[PIPE_R]);
                }

                do_external(argv);
            }
        }
        else
            printf("%s: can't found.\n", argv[0]);
    }

    return 0;
}
