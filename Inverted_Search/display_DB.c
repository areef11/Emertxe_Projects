#include "header.h"

/* Function to display database to the user in the specified format */
void display_DB(word_node_t **head)
{
	word_node_t *word_node = NULL;
	table_link_t *table_node = NULL;
	int i = 0;

	for(i = 0; i < 27; i++)													//for loop to loop through all indexes
	{
		word_node = head[i];
		while(word_node != NULL)											//while loop to loop through the word_node_t linked list
		{
			printf("\033[0;34m[%d]\t\033[0;32m[%s]\t\033[0;31m%d  \033[0mfile(s) :\t", i, word_node->word, word_node->f_count);
			table_node = word_node->sub_link;
			while(table_node != NULL)										//while loop to loop through the sub_link linked list of word_node
			{															
				printf("File :\t\033[0;32m%s\t\033[0;31m%d \033[0mtime(s)\t", table_node->f_name, table_node->w_count);
				table_node = table_node->link;
			}
			printf("-> NULL\n");
			word_node = word_node->link;
		}
	}
}
