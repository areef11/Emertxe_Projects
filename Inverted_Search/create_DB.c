#include "header.h"

char g_fname[NAMELEN];													//gloabal variable to hold filename across functions

/* Fucntion to populate the word_node_t structure array Htable_head */
int create_DB(file_node_t *file_head, word_node_t **head)
{
	while(file_head)
	{
		read_datafile(head, file_head->f_name); 
		file_head = file_head->link;
	}  
	return SUCCESS;
}

/* Function to find index number for each word */
int find_index(char ch)
{
	if (isupper(ch))
	{
		return ch-65;
	}
	else if (islower(ch))
	{
		return ch-97;
	}

	return 26;
}

/* Function to insert word_node_t for word at last */
int insert_at_last_main(word_node_t **head, char *word)
{
	word_node_t *new_word = malloc(sizeof(word_node_t));				//DMA for new_word

	if (new_word == NULL)												//error check
	{
		printf("Error: Failed to allocate memory for new word\n");
		return FAILURE;
	}

	/* Initializing structure members */
	new_word->f_count = 1;
	strcpy(new_word->word, word);
	new_word->link = NULL;
	update_link_table(&new_word);

	if (*head == NULL)													//if no words present update head with new
	{
		*head = new_word;
	}
	else																//else traverse till the last node and insert new as last node
	{
		word_node_t *temp = *head;
		while (temp->link != NULL)
		{
			temp = temp->link;
		}
		temp->link = new_word;
	}

	return SUCCESS;
}

/* Function to initialize the sub_link of a particular word_node_t variable */
int update_link_table(word_node_t **word_node)
{

	table_link_t *new_table = malloc(sizeof(table_link_t));							//DMA for new_table node


	if (new_table == NULL)
	{
		printf("Error: Failed to allocate memory for new table node\n");
		return FAILURE;
	}
	
	/* Initialize the structure members and update the sub_link of the word_node with new_table */
	new_table->w_count = 1;	
	strcpy(new_table->f_name, g_fname);
	new_table->link = NULL;
	(*word_node)->sub_link = new_table;

	return SUCCESS;
}

/* To update word count if a word is found in the database */
int update_word_count(word_node_t **head)
{
	table_link_t *temp = (*head)->sub_link;

	while(temp != NULL)												//traversing through the sub_link to check if filename matches, if yes increment the word count in *head
	{
		if (!strcmp(temp->f_name, g_fname))
		{
			temp->w_count++;
			return SUCCESS;
		}
		temp = temp->link;
	}

	/* if filename does not match, increment filecount for *head & create new table_link_t node and insert as the last node in the sub_link */
	(*head)->f_count++;

	table_link_t *new = malloc(sizeof(table_link_t));
	new->w_count = 1;
	strcpy(new->f_name, g_fname);
	new->link = NULL;
	
	table_link_t *last = (*head)->sub_link;
	while(last->link != NULL)
	{
		last = last->link;
	}
	last->link = new;
	
	return SUCCESS;
}


void read_datafile(word_node_t **head, char *f_name)
{
	strcpy(g_fname, f_name);

	FILE *fptr = fopen(f_name, "r");								//open file for reading

	if(fptr == NULL)												//error check
	{
		printf("Error: Opening file %s\n", f_name);
		return;
	}

	char word[BUFF_SIZE];

	while(fscanf(fptr, "%s", word) != EOF)    						//scan till end of the file to create database for all the words present in the file
	{
		int flag = 1;
		int index = find_index(word[0]);

		if(head[index] != NULL)
		{ 
			word_node_t *wtemp = head[index];

			while(wtemp)
			{
				if(strcmp(wtemp->word, word))						//if word already exists, increment word_count and clear flag variable
					wtemp = wtemp->link;
				else
				{
					update_word_count(&wtemp);
					flag = 0;
					break;
				}
			}
		}
		/* else come out of loop and check flag variable and call insert_at_last_main to create new node of word_node_t type and insert at last */
		if (flag)		
		{
			insert_at_last_main(&head[index], word);
		}
	}

	printf("\033[0;32mSuccess : Creation of database for file \"%s\" \033[0m\n", f_name);

	fclose(fptr);													//close file pointer
}
