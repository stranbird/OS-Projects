#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "parser.h"

#include "includes/glo.h"

#include "includes/dynamic_array.h"

bool is_delimiter(const char chr) {
    char delimiters[] = {' ', '\n', '<', '>', '|', '\0'};
    int i;
    
    for (i = 0; i != strlen(delimiters); i++)
        if (chr == delimiters[i])
            return YES;
    
    return NO;
}

void getcmd(char ***argv, int *flag, int *in, int *out) {
    char chr;
    char buf[BUFSIZE];
    int len = 0;
    int argc = 0;
    char state = -1;
    

    *argv = NULL;
    
    while (YES) {
        chr = getchar();
        
        if (is_delimiter(chr)) {
            buf[len] = '\0';
            len = 0;
            
            if (strlen(buf) == 0 && chr == ' ')
                continue;
            
            if (state == '<' || state == '>') {
                if (state == '<')
                    *in = open(buf, O_RDONLY);
                else
                    *out = open(buf, O_WRONLY | O_CREAT, 0644);
                
                state = -1;
            }
            else if (strlen(buf) > 0)
                push_back(argv, &argc, buf);
            
            if (chr == '<' || chr == '>') {
                state = chr;
                continue;
            }
            
            if (chr == ' ')
                continue;
            
            push_back(argv, &argc, NULL);
            if (chr == '\n') {
                (*flag) = NORMAL;
                break;
            }
            else if (chr == '|') {
                (*flag) = TO_PIPE;
                break;
            }            
            
        }
        else {
            buf[len++] = chr;
        }
    }
}
