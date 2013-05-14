#include <stdlib.h>
#include "includes/dynamic_array.h"

void push_back(char ***argv, int *argc, const char *buf) {
	if (*argc == 0)
	    *argv = (char **)malloc(sizeof(char **));
	else
	    *argv = (char **)realloc(*argv, sizeof(char **) * (*argc + 1));
    
    if (buf) {
        (*argv)[(*argc)] = (char *)malloc(sizeof(char) * strlen(buf));
        strcpy((*argv)[(*argc)++], buf);
    }
    else {
        (*argv)[(*argc)++] = NULL;
    }
}