#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <assert.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <errno.h>

#include "includes/glo.h"
#include "includes/builtin.h"
#include "includes/do_external.h"

#define YES 1
#define NO 0

#define REDIRECT_OUTPUT 1

#define PIPE_W 1
#define PIPE_R 0

int o_stdout, o_stdin, fd;

void reset() {
    // reset the I/O redirect etc.
    close(1);
    dup(o_stdout);
    
    close(0);
    dup(o_stdin);
}

void init() {
	getcwd(PWD, sizeof(PWD)/sizeof(char));
	strcpy(PROMPT, "=>: ");
    
    o_stdout = dup(1);
    o_stdin = dup(0);
    reset();
}

int is_cmd(const char *a_cmd, const char *b_cmd) {
	return !strcmp(a_cmd, b_cmd);
}

int is_executable(const char *a_cmd) {
	return YES;
}

int get_cmd(char ***argv, int *_to_pipe) {
    char buf[255];
    char ch;
    int i = 0, j = 0;
    int to_pipe = *_to_pipe;
    
    int state = 0;

    free(*argv);
    *argv = NULL;
    
    if (!to_pipe)
        printf("%s", PROMPT);

    while ((ch = getchar())) {
        if (ch == ' ' || ch == '\n' || ch == '<' || ch == '>' || ch == '|') {

            buf[j] = '\0';
            j = 0;
            
            if (strlen(buf) == 0) {
                if (ch == ' ')
                    continue;
            }
            else {
                if (state != 0) {
                    if (state == 1) {
                        close(0);
                        fd = open(buf, O_RDONLY);
                    }
                    else if (state == 2) {
                        close(1);
                        fd = open(buf, O_WRONLY | O_CREAT, 0644);
                    }
                    
                    state = 0;
                }
                else {
                    
                    if (*argv == NULL)
                        *argv = (char **)malloc(sizeof(char **));
                    else
                        *argv = (char **)realloc(*argv, sizeof(char **) * (i + 1));
                    
                    (*argv)[i] = (char *)malloc(strlen(buf) + 1);
                    
                    strcpy((*argv)[i], buf);
                    
                    i++;
                    
                }
            }
            
            
            if (ch == '<') {
                state = 1;
            }
            else if (ch == '>') {
                state = 2;
            }
            else if (ch == '\n' || ch == '|') {
                to_pipe = (ch == '|');
                *argv = (char **)realloc(*argv, sizeof(char **) * (i + 1));
                (*argv)[i] = NULL;
                break;
            }
        }
        else {
            buf[j++] = ch;
        }
    }
    
    *_to_pipe = to_pipe;

	return 0;
}


int main() {
	char **cmd_s = NULL;
	int argc;
    int stat_loc;
    int fd[2];
    pid_t pid;
    int to_pipe = 0, from_pipe = -1;

        
	init();

    printf("main: %d\n", getpid());

	get_cmd(&cmd_s, &to_pipe);

        
	while (is_cmd(cmd_s[0], "exit") == 0) {
        
        if (to_pipe) {
            pipe(fd);            
        }

        if ((pid = fork()) != 0) {

            if (to_pipe) {
                from_pipe = dup(fd[PIPE_R]);
                close(fd[PIPE_R]);
            }
            else
                from_pipe = -1;
            
            from_pipe = 10;
            
            printf("parent = %d\n", getpid());
            
            
            waitpid(pid, &stat_loc, 0);

        }
        else {
            printf("%d\n", from_pipe);
            printf("%d %s\n", getpid(), cmd_s[0]);
//            if (from_pipe != -1) {
//                close(PIPE_R);
//                assert(from_pipe == -1);
//                assert(dup(from_pipe) == PIPE_R);
//                close(from_pipe);
//            }
//
//            if (to_pipe) {
//                close(fd[PIPE_R]);
//                close(PIPE_W);
//                dup(fd[PIPE_W]);
//                close(fd[PIPE_W]);
//            }

        	// internal commands
            if (is_cmd(cmd_s[0], "pwd"))
            	do_pwd(cmd_s);
            else if (is_cmd(cmd_s[0], "cd")) 
            	do_cd(cmd_s);
            else if (is_cmd(cmd_s[0], "ls"))
            	do_ls(cmd_s);
            // external commands
            else if (is_executable(cmd_s[0])) {
            	do_external(cmd_s);
            }
            else {
                printf("%s: not found\n", cmd_s[0]);
            }
        }
        
        printf("%d %d\n", getpid(), from_pipe);

        if (!to_pipe)
            reset();

		get_cmd(&cmd_s, &argc);
	}
    
	return 0;
}