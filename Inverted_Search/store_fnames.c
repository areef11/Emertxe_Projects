#include "header.h"

/* Function to loop through all the files passed to validate and store them into file_head */
void validate_n_store_filenames(file_node_t **file_head, char *filenames[])
{
	int status, i = 1;
	while (filenames[i] != NULL)											//while loop to loop through all files in argv[] 	
	{
		status = isFileValid(filenames[i]);
		if(status == NOT_PRESENT)
		{
			printf("\033[0;31m");
			printf("Error : The file %s is not present.\nSo we are not adding this file into the list.\n", filenames[i]);
			printf("\033[0m");
		}
		else if (status == FILE_EMPTY)
		{
			printf("\033[0;31m");
			printf("Error : The file %s is an empty file.\nSo we are not adding this file into the list.\n", filenames[i]);
			printf("\033[0m");
		}
		else
		{
			if (store_filenames_to_list(filenames[i], file_head) == REPEATED)
			{
				printf("\033[0;31m");
				printf("Error : The file %s is repeated.\nSo we are not adding this file into the list.\n", filenames[i]);
				printf("\033[0m");
			}
			else
			{
				printf("\033[0;32m");
				printf("Success : Added file %s to the list\n", filenames[i]);
				printf("\033[0m");
			}
		}
		i++;
	}
}

/* Function to check if file exists and is file empty */
int isFileValid(char *fname)
{
	FILE *fp = fopen(fname, "r");

	if(fp == NULL)
	{
		return NOT_PRESENT;
	}

	fseek (fp, 0, SEEK_END);
	if (ftell(fp) == 0)
	{
		fclose(fp);
		return FILE_EMPTY;
	}

	fclose(fp);
	return SUCCESS;
}

/* Function to store valid files into file_node_t */
int store_filenames_to_list(char *f_name, file_node_t **file_head)
{
	file_node_t *new = malloc(sizeof(file_node_t));

	if(new == NULL)
	{
		return FAILURE;
	}

	strcpy(new->f_name, f_name);
	new->link = NULL;

	if(*file_head == NULL)
	{
		*file_head = new;
		return SUCCESS;
	}

	file_node_t *temp = *file_head, *prev;

	while (temp != NULL)
	{
		if(strcmp(temp->f_name, f_name) != 0)
		{
			prev = temp;
			temp = temp->link;
		}
		else
		{
			free(new);
			return REPEATED;
		}
	}
	prev->link = new;

	return SUCCESS;
}
