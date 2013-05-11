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

void reset() {
    // reset the I/O redirect etc.
}

void init() {
	getcwd(PWD, sizeof(PWD)/sizeof(char));
	strcpy(PROMPT, "=>: ");

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
    char **ptr;

    free(*argv);
    *argv = NULL;


    printf("%s", PROMPT);

    while (ch = getchar()) {
        if (ch == ' ' || ch == '\n' || ch == '<' || ch == '>') {

            buf[j] = '\0';

            if (*argv == NULL)
                *argv = (char **)malloc(sizeof(char **));
            else
                *argv = (char **)realloc(*argv, sizeof(char **) * (i + 1));

            (*argv)[i] = (char *)malloc(strlen(buf) + 1);

            strcpy((*argv)[i], buf);

            i++;
            j = 0;

            if (ch == '\n') {
                *argv = (char **)realloc(*argv, sizeof(char **) * (i + 1));
                (*argv)[i] = NULL;
                break;
            }
            else if (ch == '<') {

            }
            else if (ch == '>') {

            }
        }
        else {
            buf[j++] = ch;
        }
    }
    
    printf("%ld\n", sizeof(buf));
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