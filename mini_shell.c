/*PROJECT : IMPLEMENT MINIALISTIC SHELL
Description:Create a minialistic shell that performs similar to the existing shell, 
using system calls w.r.t process creation,signal handling,process synchronisation
exit status.*/

#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include "declaration.h"

/*Handler function for SIGINT*/
void sig_handler(int signum, siginfo_t *info, void *nouse)
{
	putchar('\n');
	flag1 = 1;
}

/*Handler function for SIGTSTP*/
void sig_handler1(int signum, siginfo_t *info, void *nouse)
{
	flag2 = 1;
	putchar('\n');
	printf("PID %d\n", child_pid);
	if(child_pid != 0)
	{
		kill(child_pid, SIGSTOP);
	}
}

/*Function to print the welcome mini_shell command prompt*/
void terminal_prompt()
{

	printf("\n\n********************************************************************************\n\n");
	printf("\t\t\tWELCOME TO MINI_SHELL PROMPT\n");
	printf("\t\t\tAuthor : Manjunath\n");
	printf("\n\n*********************************************************************************\n\n");
}

/* Function to fetch the current working path*/
void path()
{
	printf("%s :> \n",getcwd(currentdirectory, 1024)); //getcwd is system function which gives the current working directory
}

/*Fetching the commands entered*/
int command_checking(char *command)
{
	int i = 0, j = 0, k = 0, flag = 1;
	short ret;

	char *command_option[MAX_CMD_LENGTH];
	char *cd_cmd;

	memset(command_option, 0, MAX_CMD_LENGTH *sizeof(char *));

	/* if any command present then enters the while loop*/
	while(command[i] != '\0')
	{
		if(command[i] == ' ')      		 //checks if any space is there
		{
			if(!flag)           					     // enters if loop only if flag is 0
			{
				flag = 1;
				command[i] = '\0';				    //the ith position is added null
				command_option[k] = malloc(10); 			//malloc an array of k
				strcpy(command_option[k], &command[j]);             //copy the content that is stored in j to the kth array
				i++;                                                // increment i
				k++;                                                // increment k
			}
			else
			{
				i++;
			}
		}

		else
		{
			if(flag)    				  // if flag is 1 it enter the loop
			{
				j = i;   			// j is inintalized with the value of i
			}
			flag = 0;
			i++;
		}
	}

	command_option[k] = malloc(10);
	strcpy(command_option[k], &command[j]);                    //copy the jth address value in kth array

	/*comparing the echo and $ command entered*/ 
	if((!strcmp(command_option[0], "echo")) && (*command_option[1] == '$'))
	{
		child_status = echo_function(command_option);         //echo function call
	}

	/*comparing of n bytes of the PS1 in command line*/
	else if(!strncmp(command_option[0], "PS1=", 4))
	{
		child_status = ps1_function(command_option);
	}
	/* comapring for the cd command enterd through command line*/
	else if(strcmp(command_option[0], "cd") == 0)
	{
		change_directory(command_option[1]);
	}
	/*for the exit command*/
	else if(!strcmp(command_option[0], "exit"))
	{
		exit(0);
	}
	/* other then entered command go for exe_function call*/
	else
	{
		ret = exe_function(command_option);
		for(i = 0; i <= k; i++)
		{
			free(command_option[i]);
		}
		return ret;
	}
	return 0;
}

/*Change directiry function definition*/
int change_directory(char *cmd)
{
	char path[100];
	char cwd[100];

	strcpy(path, cmd);

	getcwd(cwd, sizeof(cwd));

	strcat(cwd, "/");
	strcat(cwd, cmd);
	strcat(cwd, "/");

	chdir(cwd);
	return 0;
}

/*PS1 function definition*/
int ps1_function(char **command_option)
{
	if((command_option[0][4]) != ' ')
		strcpy(PS1, command_option[0] + 4);
	else
		return(exe_function(command_option));

	return 0;
}

/*execute function definition*/
int exe_function(char *command_option[])

	child_pid = fork();

	switch(child_pid)
	{
		case -1:
			perror("Process");
			return 1;

		case 0:
			execvp(command_option[0], command_option);
			perror(command_option[0]);
			exit(127);

		default :
			wait(&child_status);
			return child_status;
	}
}

/*echo function definition*/
int echo_function(char *command_option[])
{
	if(!strcmp(command_option[1], "$$"))
		printf("%u\n", getpid());

	else if(!strcmp(command_option[1], "$?"))
		printf("%d\n", child_status);
	else if(!strcmp(command_option[1], "$SHELL"))
		printf("%s/ %s\n", getenv("PWD"), "msh");
	else
		return(exe_function(command_option));

	return 0;
}

/* Main program */
int main(int argc, char *argv[])
{
	char command[MAX_CMD_LENGTH];

	system("clear");

	terminal_prompt(); //terminal function call to print the prompt
	path();            //fetching the path function for the current directory

	strcpy(PS1, "msh\0");  //copying the string to PS1
	PS2 = '>';             

	struct sigaction act1, act2, act3, act4;

	memset(&act1, 0, sizeof(act1));
	act1.sa_sigaction = sig_handler;
	sigaction(SIGINT, &act1, &act2);


	memset(&act3, 0, sizeof(act3));
	act3.sa_sigaction = sig_handler1;
	sigaction(SIGTSTP, &act3, &act4);
	while(1)
	{

		write(1, PS1, strlen(PS1));       //writing the PS1 in stdout
		write(1, &PS2, 1);	              //writing PS2 in stdout
		__fpurge(stdin);

		memset(command, '\0', MAX_CMD_LENGTH);		//command is an Array[];
		__fpurge(stdin);

		scanf("%[^\n]", command);

		if(flag1)
		{
			flag1 = 0;
			continue;
		}

		if(flag2)
		{
			flag2 = 0;
			child_pid = 0;
			continue;
		}


		if(*command == '\0')
		{
			continue;
		}
		else
		{
			child_status = command_checking(command);
		}

	}
}

