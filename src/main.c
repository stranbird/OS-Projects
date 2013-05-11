#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <errno.h>

#include "includes/glo.h"
#include "includes/builtin.h"
#include "includes/do_external.h"

#define YES 1
#define NO 0

#define REDIRECT_OUTPUT 1

int o_stdout, fd;

void reset() {
    // reset the I/O redirect etc.
    close(1);
    dup(o_stdout);
}

void init() {
	getcwd(PWD, sizeof(PWD)/sizeof(char));
	strcpy(PROMPT, "=>: ");
    
    o_stdout = dup(1);
    reset();
}

int is_cmd(const char *a_cmd, const char *b_cmd) {
	return !strcmp(a_cmd, b_cmd);
}

int is_executable(const char *a_cmd) {
	return YES;
}

int get_cmd(char ***argv, int *argc) {
    char buf[255];
    char ch, last_ch;
    int i = 0, j = 0;
    
    int state = 0;

    free(*argv);
    *argv = NULL;
    


    printf("%s", PROMPT);

    while (ch = getchar()) {
        if (ch == ' ' || ch == '\n' || ch == '<' || ch == '>') {

            buf[j] = '\0';
            j = 0;
            
            if (strlen(buf) == 0) {
                if (ch == ' ')
                    continue;
            }
            else {
                if (state != 0) {
                    if (state == 1) {
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
            else if (ch == '\n') {
                *argv = (char **)realloc(*argv, sizeof(char **) * (i + 1));
                (*argv)[i] = NULL;
                break;
            }
        }
        else {
            buf[j++] = ch;
        }
    }
    
    *argc = i;

	return 0;
}


int main() {
	char **cmd_s = NULL;
	int argc;
    int i;
    
	init();

	get_cmd(&cmd_s, &argc);
    
	while (is_cmd(cmd_s[0], "exit") == 0) {
    
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

        reset();

		get_cmd(&cmd_s, &argc);
	}

	return 0;
}