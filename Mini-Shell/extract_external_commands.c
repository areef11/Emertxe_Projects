#include "main.h"

void extract_external_commands(char **external_commands)
{
	//Variable declaration
	char ch;
	char buffer[25] = {'\0'};
	int i = 0, j = 0;


	//Open the file
	int fd = open("external_commands.txt", O_RDONLY);
	//Error check
	if (fd == -1)
	{
		printf("Error opening the file\n");
		exit(0);
	}

	//Read the commands from the file
	while(read(fd, &ch, 1) > 0)
	{
		if (ch != '\n')
		{
			buffer[i++] = ch;
		}
		else
		{
			//Allocate sufficient memory 
			external_commands[j] = calloc(strlen(buffer)+1, sizeof(char));
			
			//Copy the contents
			strcpy(external_commands[j++], buffer);
			
			//Append the null character at the end
			external_commands[j] = '\0';
			
			//Clear the buffer
			memset(buffer, '\0', 25);

			i = 0;
		}
	}
}
