#include <stdio.h>
#include <unistd.h>

#include "../includes/glo.h"

void do_cd(char **argv) {
    puts(argv[1]);
	if (chdir(argv[1]) == -1) {
	    printf("chdir: can't cd to %s\n", argv[1]);
	}
	else {
	    getcwd(PWD, sizeof(PWD)/sizeof(char));
        puts(PWD);
    }
}