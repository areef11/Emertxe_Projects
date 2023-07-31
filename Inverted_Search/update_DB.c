#include "header.h"

int update_DB(file_node_t *head_file, word_node_t **head_word, char *filename) 
{	
	/* checking if the file is already in the database */
	file_node_t *curr_file = head_file;
	while (curr_file != NULL) 
	{
		if (strcmp(curr_file->f_name, filename) == 0) 
		{
			return REPEATED;								//file already exists in database, return REPEATED
		}
		curr_file = curr_file->link;
	}
	

	// if file is not in the database, add it
	file_node_t *new_file = (file_node_t *)malloc(sizeof(file_node_t));
	
	if (new_file == NULL) 
	{
		return FAILURE; 
	}

	strcpy(new_file->f_name, filename);
	new_file->link = head_file;
	curr_file = new_file;

	//read the file and add its words to the database
	read_datafile(head_word, filename);

	return SUCCESS; 
}
