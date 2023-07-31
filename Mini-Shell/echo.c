#include "main.h"

void echo(char *input_string, int status)
{
	//To print Exit status of the last command
	if (strncmp("echo $?", input_string, 7) == 0)
		printf("%d\n", status); 
	
	//To print PID of minishell
	if (strncmp("echo $$", input_string, 7) == 0)
		printf("%d\n", getpid());
	
	//To print executable path
	if (strncmp("echo $SHELL", input_string, 11) == 0)
		system("pwd");


}
