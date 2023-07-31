#include "header.h"

/* Function to search a word in the database */
int search_DB(word_node_t **head, char *word)
{
	int index = find_index(word[0]); 				//get index of the first character
	word_node_t *ptr = head[index]; 				//get the pointer to the first node in the linked list
	while (ptr != NULL)
	{
		/* compare word with the current node's word, if found return SUCCESS else loop till the last node & if not found return NOT_PRESENT */
		if (strcmp(ptr->word, word) == 0) 		
		{
			printf("\033[0;31mSuccess : Word \033[1;32m\"%s\"\033[0;31m found in database and it is present in \033[0;32m%d file(s)\033[0m\n", word, ptr->f_count);
			table_link_t *sub_ptr = ptr->sub_link;
			while (sub_ptr != NULL)
			{
				printf("\033[0;31mIn file  \033[1;32m\"%s\" :\033[0;31m %d time(s)\n\033[0m", sub_ptr->f_name, sub_ptr->w_count);
				sub_ptr = sub_ptr->link;
			}
			return SUCCESS;
		}
		ptr = ptr->link;
	}
	return NOT_PRESENT;
}
