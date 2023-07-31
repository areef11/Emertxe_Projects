#include "main.h"

void execute_internal_commands(char *input_string)
{
	//Compare input_string and execute exit command
	if(strncmp("exit", input_string, 4) == 0)
		exit(3);

	//Compare input_string and execute pwd command
	if(strncmp("pwd", input_string, 3) == 0)
		system("pwd");

	//Compare input string and execute cd command
	if(strncmp("cd", input_string, 2) == 0)
	{
		//Neglect spaces in the input_string for cd command
		int i = 2;
		while (isspace(input_string[i]))
			i++;

		chdir(&input_string[i]);
	}
}
