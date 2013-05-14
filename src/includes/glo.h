#define MAX_PATH_LEN 255
#define MAX_CMD_LEN 255
#define MAX_PROMPT_LEN 255

#define YES 1
#define NO 0

char PWD[MAX_PATH_LEN];
char PROMPT[MAX_PROMPT_LEN];

int o_stdout, o_stdin;
int fd[2], ofd[2];
int is_pipe;

typedef int bool;