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

    int o_stdout, fd;

    int state = 0;;
    
    int is_inside_quotation = 0;
    memset(buf, 0, sizeof(buf));
    
    
	printf("%s", PROMPT);

    if (*argv)
        free(*argv);
    
	*argv = (char **)malloc(sizeof(char **) * 8);
        
    while ((ch = getchar()) != '\n' || last_ch == '\\') {
        if ((ch != '<' && ch != '>' && ch != ' ') || is_inside_quotation) {
            last_ch = ch;
            buf[j++] = ch;
            
            if (ch == '"') {
                is_inside_quotation = 1 - is_inside_quotation;
            }

            puts("1");
        }
        else if (ch == '<') {
            puts("2");
        }
        else if (ch == '>') {
            state = REDIRECT_OUTPUT;
            puts("3");
        }
        // Got a valid token, could be a file_path of I/O redirection.
        else {
            buf[j] = '\0';

            printf("current state: %d\n", state);

            if (state == REDIRECT_OUTPUT) {
                o_stdout = dup(1);
                close(1);
                fd = open(buf, O_WRONLY | O_CREAT);
            }
            else {

                (*argv)[i] = (char *)malloc(strlen(buf) + 1);
                strcpy((*argv)[i], buf);
                memset(buf, 0, sizeof(buf));
                
                i++;
            }

            j = 0;
            puts("4");
        }
    }
    
    buf[j] = '\0';
    
    (*argv)[i] = (char *)malloc(strlen(buf) + 1);
    strcpy((*argv)[i], buf);

    for (int j = 0; j <= i; j++) {
        printf("%s\n", (*argv)[j]);
    }
    
	return 0;
}


int main() {
	char **cmd_s = NULL;
	int argc;
    
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