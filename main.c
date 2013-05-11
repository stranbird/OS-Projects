#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

#include <sys/stat.h>

#include <errno.h>

#include "glo.h"

void do_ls() {
	const char *dir_path = ".";
	struct dirent *dp;
	char *file_path;
	struct stat buf;


	DIR *dirp = opendir(dir_path);

	printf("%8s\t%8s\t%20s\t%8s\t%12s\n", "inode", "nlink", "name", "size", "modified");

	while ((dp = readdir(dirp)) != NULL) {
	  lstat(dp->d_name, &buf);
	  // printf("%d\n", buf.st_mode);
	  printf("%8ld\t%8ld\t%20s\t%8ld\t%12ld\n", (long int)buf.st_ino, (long int)buf.st_nlink, dp->d_name, (long int)buf.st_size, buf.st_mtimespec.tv_sec);
	}
	closedir(dirp);
}

int is_cmd(const char *a_cmd, const char *b_cmd) {
	return !strcmp(a_cmd, b_cmd);
}

void init() {
	getcwd(PWD, sizeof(PWD)/sizeof(char));
    
	strcpy(PROMPT, "=>: ");
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
    
        if (is_cmd(cmd_s[0], "pwd")) {
            printf("%s\n", PWD);
        }
        else if (is_cmd(cmd_s[0], "cd")) {
            puts(cmd_s[1]);
            if (chdir(cmd_s[1]) == -1) {
                printf("chdir: can't cd to %s\n", cmd_s[1]);
            }
            else
                getcwd(PWD, sizeof(PWD)/sizeof(char));
        }
        else if (is_cmd(cmd_s[0], "ls"))
        	do_ls();
        else {
            printf("%s: not found\n", cmd_s[0]);
        }

		get_cmd(&cmd_s, &argc);
	}

	return 0;
}