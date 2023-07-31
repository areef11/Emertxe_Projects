#include "main.h"

pid_t pid;

void scan_input(char *prompt, char *input_string)
{
	/* Variable declaration */
	char *command;
	int command_type, status;

	while (1)
	{
		// Display the prompt
		printf(ANSI_COLOR_RED"[%s]$"ANSI_COLOR_RESET, prompt);
		
		//Clear the output buffer
		fflush(stdout);

		//Read the input from the user
		scanf("%[^\n]s", input_string);
		
		//Clear the input buffer
		getchar();

		//Customize the prompt
		if(!strncmp("PS1=", input_string,4))
		{
			if (input_string[4] != ' ')
			{
				strcpy(prompt, &input_string[4]);

				//Clear the input string
				memset(input_string, '\0', 25);
				continue;
			}
			printf("Command not found\n");
		}

		//To get the command
		command = get_command(input_string);
		//printf("Command is %s\n", command);	

		command_type = check_command_type(command);
		//printf("Command_type is %d\n", command_type);

		if(command_type == EXTERNAL)
		{
			//Create child process */
			pid = fork();
			/* In child process */
			if(pid == 0)
			{
				/* Execute the given command */
				int ret = system(input_string);
				if(ret == 0)
					exit(0);
				else
					exit(1);
			}
			/* In parent process */
			else if(pid > 0)
			{
				/* Wait for child process completion */
				wait(&status);
				if(WIFEXITED(status))
				{
					printf("Child with pid %d terminated normally\n", pid);
				}
			}
			else
			{
				printf("Fork system call failed.\n");
				exit(2);
			}
		}

		//Call the function to implement echo commands
		echo(input_string, status);	

		//Call the function to implement builtin commands
		execute_internal_commands(input_string);
	}
}
