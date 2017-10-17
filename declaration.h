#include<stdio.h>


#define BUFFER_MAX_LENGTH 1024
#define MAX_CMD_LENGTH 100

static char CurrentDirectory[1024];
static char *currentdirectory;
static char input_command = '\0';
static char buffer[BUFFER_MAX_LENGTH];
static int bufferChars = 0;

static char *commandArgv[5];
static int commandArgc = 0;
char PS1[100];
char PS2;
int child_status, flag1, flag2;
pid_t child_pid;
