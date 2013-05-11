#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

#include <sys/stat.h>

#include <errno.h>

#include "includes/glo.h"

#define YES 1
#define NO 0

void init() {
	getcwd(PWD, sizeof(PWD)/sizeof(char));
    
	strcpy(PROMPT, "=>: ");
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
    
    int is_inside_quotation = 0;
    memset(buf, 0, sizeof(buf));
    
    
	printf("%s", PROMPT);

    if (*argv)
        free(*argv);
    
	*argv = (char **)malloc(sizeof(char **) * 8);
    

    
    while ((ch = getchar()) != '\n' || last_ch == '\\') {
        if (ch != ' ' || is_inside_quotation) {
            last_ch = ch;
            buf[j++] = ch;
            
            if (ch == '"') {
                is_inside_quotation = 1 - is_inside_quotation;
            }
        }
        else {
            buf[j] = '\0';
            
            (*argv)[i] = (char *)malloc(strlen(buf) + 1);
            strcpy((*argv)[i], buf);
            memset(buf, 0, sizeof(buf));
            
            i++;
            j = 0;
        }
    }
    
    buf[j] = '\0';
    
    (*argv)[i] = (char *)malloc(strlen(buf) + 1);
    strcpy((*argv)[i], buf);
    
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

		get_cmd(&cmd_s, &argc);
	}

	return 0;
}