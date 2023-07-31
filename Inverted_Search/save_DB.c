#include "header.h"

/* Function to save the database to the file given by user in the specified format */
int save_DB(word_node_t **head, char *f_name)
{
	word_node_t *word_node = NULL;
	table_link_t *table_node = NULL;
	int i = 0;
	FILE *fp;

	if ((fp = fopen(f_name, "w")) == NULL)													//opening file f_name for writing and performing error check 
	{
		printf("Error opening file %s", f_name);
		return FAILURE;
	}

	for (i = 0; i < 27; i++) {
		word_node = head[i];
		while (word_node != NULL) {
			fprintf(fp, "#%d\n%s;%d;", i, word_node->word, word_node->f_count);				//using fprint to write to a file instead of stdout
			table_node = word_node->sub_link;
			while (table_node != NULL) {
				fprintf(fp, "%s;%d;", table_node->f_name, table_node->w_count);
				table_node = table_node->link;
			}
			fprintf(fp, "#\n");
			word_node = word_node->link;
		}
	}
	fclose(fp);

	return SUCCESS;
}
