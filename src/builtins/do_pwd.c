#include <stdio.h>

#include "../includes/glo.h"

#include "../includes/builtin.h"

void do_pwd(char **argv) {
    getcwd(PWD, sizeof(PWD)/sizeof(char));
    puts(PWD);
}
