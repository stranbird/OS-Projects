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

int o_stdout, o_stdin;
int fd[2], ofd[2];

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

int get_cmd(char ***argv, int *is_pipe) {
    char buf[255];
    char ch;
    int i = 0, j;
    int rfd;
    
    memset(buf, 0, sizeof(buf));
    
    int state = 0;

    free(*argv);
    *argv = NULL;
    
    if (!(*is_pipe))
        printf("%s", PROMPT);

    j = 0;
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
                        rfd = open(buf, O_RDONLY);
                    }
                    else if (state == 2) {
                        close(1);
                        rfd = open(buf, O_WRONLY | O_CREAT, 0644);
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
                *is_pipe = (ch == '|');
                if (i == 0) {
                    return get_cmd(argv, is_pipe);
                }
                *argv = (char **)realloc(*argv, sizeof(char **) * (i + 1));
                (*argv)[i] = NULL;
                break;
            }
        }
//        else if (ch == \xff) {
        
//        }
        else {
            buf[j++] = ch;
        }
    }

	return 0;
}

int isMainProcess(pid_t mpid) {
    return mpid == getpid();
}

int is_internal(const char *cmd) {
    return (strcmp(cmd, "pwd") && strcmp(cmd, "cd") && strcmp(cmd, "ls")) == 0;
}

int is_external(const char *cmd) {
    return access(cmd, X_OK) != -1;
}


int main() {
	char **cmd_s = NULL;
    int stat_loc;
    int is_pipe = 0;
    
//    const pid_t main_process = getpid();

        
	init();
    
    ofd[PIPE_R] = -1;


	get_cmd(&cmd_s, &is_pipe);

        
	while (is_cmd(cmd_s[0], "exit") == 0) {
        
        if (is_internal(cmd_s[0])) {
            if (ofd[PIPE_R] != -1) {
                close(ofd[PIPE_W]);
                close(PIPE_R);
                dup(ofd[PIPE_R]);
                close(ofd[PIPE_R]);
            }
            
            if (is_pipe) {
                pipe(fd);
                
                ofd[PIPE_R] = dup(fd[PIPE_R]);
                ofd[PIPE_W] = dup(fd[PIPE_W]);
                
                close(fd[PIPE_R]);
                close(PIPE_W);
                dup(fd[PIPE_W]);
                close(fd[PIPE_W]);
            }
            else
                ofd[PIPE_R] = -1;
            
            if (is_cmd(cmd_s[0], "pwd"))
            	do_pwd(cmd_s);
            else if (is_cmd(cmd_s[0], "cd")) {
              puts("cd");
            	do_cd(cmd_s);
            }
            else if (is_cmd(cmd_s[0], "ls")) {
              puts("here");
            	do_ls(cmd_s);
            }
        }
        else if (is_external(cmd_s[0])) {
            do_external(cmd_s);
        }
        else
            printf("%s: not found\n", cmd_s[0]);

//        if ((pid = fork()) != 0) {
//            
////            if (ofd[PIPE_R] == -1)
//                waitpid(-1, &stat_loc, 0);
//
//            if (is_pipe) {
//                ofd[0] = fd[0];
//                ofd[1] = fd[1];
//            }
//            else {
//                ofd[PIPE_R] = -1;
//            }
//            
////            waitpid(-1, &stat_loc, 0);
//        }
//        else {
//            if (ofd[PIPE_R] != -1) {
//                close(ofd[PIPE_W]);
//                close(PIPE_R);
//                assert(dup(ofd[PIPE_R]) == PIPE_R);
//                close(ofd[PIPE_R]);
//            }
//
//            if (is_pipe) {
//                close(fd[PIPE_R]);
//                close(PIPE_W);
//                dup(fd[PIPE_W]);
//                close(fd[PIPE_W]);
//            }
//
//        	// internal commands
//            if (is_cmd(cmd_s[0], "pwd"))
//            	do_pwd(cmd_s);
//            else if (is_cmd(cmd_s[0], "cd")) x{
//                puts("cd");
//            	do_cd(cmd_s);
//            }
//            else if (is_cmd(cmd_s[0], "ls")) {
//                puts("here");
//            	do_ls(cmd_s);
//            }
//            // external commands
//            else if (is_executable(cmd_s[0])) {
//            	do_external(cmd_s);
//            }
//            else {
//                printf("%s: not found\n", cmd_s[0]);
//            }
//            
//            printf("should be here2: %s\n", cmd_s[0]);
//            
//            return 0;
//        }
        
//        if (is_pipe) {
//            ofd[PIPE_R] = fd[PIPE_R];
//            ofd[PIPE_W] = fd[PIPE_W];
//        }
//        else
//            ofd[PIPE_R] = -1;
        
        if (!is_pipe)
            reset();
        
		get_cmd(&cmd_s, &is_pipe);
	}
    
	return 0;
}