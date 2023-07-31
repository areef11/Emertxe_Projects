#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
#define SUCCESS 0
#define FAILURE -1
#define NOELEMENT -2
#define EMPTYLIST -3
#define REPEATED -4
#define FILE_EMPTY -5
#define NOT_PRESENT -6

#define SIZE 27
#define BUFF_SIZE 255
#define NAMELEN 50

/* Structures to be used in the program */
typedef struct table_node
{
	char f_name[NAMELEN];
	int w_count;
	struct table_node *link;
}table_link_t;

typedef struct node
{
	char word[NAMELEN];
	struct node *link;
	table_link_t *sub_link;
	int f_count;
}word_node_t;

typedef struct file_node
{
	char f_name[NAMELEN];
	struct file_node *link;
}file_node_t;

/* File Validation */
void validate_n_store_filenames(file_node_t **, char *filenames[]);
int isFileValid(char *);
int store_filenames_to_list(char *f_name, file_node_t **file_head);

/* Create Database */
int create_DB(file_node_t *file_head, word_node_t **head);
void read_datafile(word_node_t **, char *f_name);
int insert_at_last_main(word_node_t **head, char *word);
int update_link_table(word_node_t **word_node);
int update_word_count(word_node_t **head);
int find_index(char ch);

/* Display DB */
void display_DB(word_node_t **head);

/* Search DB */
int search_DB(word_node_t **head, char *word);

/* Save DB */
int save_DB(word_node_t **head, char *fname);

/* Update DB */
int update_DB(file_node_t *, word_node_t **, char *);

#endif
