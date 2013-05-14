#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "parser.h"

#include "includes/pipe.h"
#include "includes/glo.h"
#include "includes/builtin.h"
#include "includes/do_external.h"

int o_stdin, o_stdout;
int fd[2], ofd[2];


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
    
    for (int i = 0; i != sizeof(internal_cmd) / sizeof(char *); i++)
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

void reset_pipe() {
    fd[PIPE_R] = PIPE_R;
    fd[PIPE_W] = PIPE_W;
    
    close(PIPE_R);
    dup(o_stdin);
    
    close(PIPE_W);
    dup(o_stdout);
}

int main()
{
    char **argv;
    bool flag = NORMAL;
    int stat;
    pid_t pid;
    
    o_stdin = dup(PIPE_R);
    o_stdout = dup(PIPE_W);
    
    reset_pipe();
    
    ofd[PIPE_R] = -1;
    
    while (YES) {
        type_prompt(flag);
        
        reset_pipe();
        
        getcmd(&argv, &flag, &fd[PIPE_R], &fd[PIPE_W]);
        
        
        if (flag == TO_PIPE) {
            pipe(fd);
        }
        else if (flag == NORMAL) {
            if (fd[PIPE_R] != PIPE_R) {
                close(PIPE_R);
                dup(fd[PIPE_R]);
                close(fd[PIPE_R]);
                fd[PIPE_R] = PIPE_R;
            }
            if (fd[PIPE_W] != PIPE_W) {
                close(PIPE_W);
                dup(fd[PIPE_W]);
                close(fd[PIPE_W]);
                fd[PIPE_W] = PIPE_W;
            }
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
                    close(fd[PIPE_R]);
                    close(PIPE_W);
                    dup(fd[PIPE_W]);
                    close(fd[PIPE_W]);
                }
                
                if (ofd[PIPE_R] != PIPE_R) {
                    close(PIPE_R);
                    dup(ofd[PIPE_R]);
                    close(ofd[PIPE_R]);
                }
                
                do_external(argv);
            }
        }
        else
            printf("%s: can't found.\n", argv[0]);
    }

    return 0;
}