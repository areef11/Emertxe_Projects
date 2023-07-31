/* 	
~~~~~~ Documentation ~~~~~~~~
Name			 : Areef Ahmad
Date			 : 15-02-2023
Description		 : DS Project to implement inverted index program
Sample Execution : ./a.out 1.txt 2.txt file.txt empty.txt
Success : Added file 1.txt to the list                                 
Success : Added file 2.txt to the list                                 
Error : The file file.txt is not present.                              
So we are not adding this file into the list.                          
Error : The file empty.txt is an empty file.                           
So we are not adding this file into the list.                          
----------MENU----------                                               
1. Create 
2. Display Database                                                    
3. Search Database                                                     
4. Update Database                                                     
5. Save Database                                                       
6. Exit                                                                
Please enter your choice : 1                                           
Success : Creation of database for file "1.txt"                        
Success : Creation of database for file "2.txt"                        
Database Created Successfully                                          
----------MENU----------                                               
1. Create Database                                                     
2. Display Database                                                    
3. Search Database                                                     
4. Update Database                                                     
5. Save Database                                                       
6. Exit  
Please enter your choice : 2                                           
[5]     [file]  2  file(s) :    File :  1.txt   1 time(s)       File : 2.txt   1 time(s)       -> NULL                                        
[6]     [Good]  1  file(s) :    File :  1.txt   1 time(s)       -> NULL
[6]     [good]  1  file(s) :    File :  2.txt   1 time(s)       -> NULL
[8]     [is]    2  file(s) :    File :  1.txt   1 time(s)       File : 2.txt   1 time(s)       -> NULL                                        
[12]    [morning]       2  file(s) :    File :  1.txt   1 time(s)       File : 2.txt   1 time(s)       -> NULL                                
[19]    [This]  2  file(s) :    File :  1.txt   1 time(s)       File : 2.txt   1 time(s)       -> NULL                                        
[26]    [1]     1  file(s) :    File :  1.txt   1 time(s)       -> NULL
[26]    [2]     1  file(s) :    File :  2.txt   1 time(s)       -> NULL
----------MENU----------                                               
1. Create Database                                                     
2. Display Database                                                    
3. Search Database                                                     
4. Update Database                                                     
5. Save Database                                                       
6. Exit              
Please enter your choice : 3                                           
Enter the word to be searched : morning                                
Success : Word "morning" found in database and it is present in 2 file(s)                                                                     
In file  "1.txt" : 1 time(s)                                           
In file  "2.txt" : 1 time(s)                                           
----------MENU----------                                               
1. Create Database                                                     
2. Display Database                                                    
3. Search Database                                                     
4. Update Database                                                     
5. Save Database                                                       
6. Exit     
Please enter your choice : 4                                           
Enter the name of the file to be added : 3.txt                         
Success : Creation of database for file "3.txt"                        
Database updated with file 3.txt                                       
----------MENU----------                                               
1. Create Database                                                     
2. Display Database                                                    
3. Search Database                                                     
4. Update Database                                                     
5. Save Database                                                       
6. Exit          
Please enter your choice : 4
Enter the name of the file to be added : 1.txt
Error: File already present in database.
----------MENU----------
1. Create Database
2. Display Database
3. Search Database
4. Update Database
5. Save Database
6. Exit                                           
Please enter your choice : 3
Enter the word to be searched : nothing                                
FAILURE : Word "nothing" not found in the database.
----------MENU----------                                               
1. Create Database                                                     
2. Display Database                                                    
3. Search Database                                                     
4. Update Database                                                     
5. Save Database                                                       
6. Exit            
Please enter your choice : 5                                           
Enter the name of the backup file : backup.txt                         
Success : Database saved into the file backup.txt.                     
----------MENU----------                                               
1. Create Database                                                     
2. Display Database                                                    
3. Search Database                                                     
4. Update Database                                                     
5. Save Database                                                       
6. Exit         
Please enter your choice : 6
-------Exiting-------
*/

#include "header.h"

int main(int argc, char *argv[])
{
	int ch;
	char word[NAMELEN], file_name[NAMELEN];

	/* checking if CLAs are passed or not */
	if (argc == 1)
	{
		printf("Error: Invalid number of arguments\nUsage: ./inverted_index <file1.txt> <file2.txt>\n");
		return FAILURE;
	}

	/* declaring file_node_t structure array and calling validate_n_store_filesnames function to store files passed as CLAs into the array */
	file_node_t *file_head = NULL;
	validate_n_store_filenames(&file_head, argv);

	if(file_head == NULL)
	{
		printf("There is no valid files\nPlease enter valid files.\n");
		return FAILURE;
	}

	/* declaring word_node_t structure array and initializing with NULL */
	word_node_t *Htable_head[27] = {NULL};

	while(1)
	{
		/* printing menu for the user to select mode of operation */
		printf("\033[0;34m");
		printf("----------MENU----------\n1. Create Database\n2. Display Database\n3. Search Database\n4. Update Database\n5. Save Database\n6. Exit\nPlease enter your choice : ");
		printf("\033[0m");
		scanf(" %d", &ch);
		switch(ch)															//switch case to call any function as per user's choice
		{
			case 1:
				if(create_DB(file_head, Htable_head) == SUCCESS)
				{
					printf("\033[1;32mDatabase Created Successfully\033[0m\n");
				}
				else
				{
					printf("\033[0;31mError : Could not create database.\033[0m\n");
				}
				break;
			case 2:
				display_DB(Htable_head);
				break;	
			case 3:
				printf("\033[0;34mEnter the word to be searched : \033[0m");
				scanf("%s", word);

				if((search_DB(Htable_head, word) == NOT_PRESENT))
				{
					printf("\033[0;31mFAILURE : Word \"%s\" not found in the database.\033[0m\n", word);
				}
				else
				{
				}
				break;
			case 4:
				printf("\033[0;34mEnter the name of the file to be added : \033[0m");
				scanf("%s", file_name);
				if((update_DB(file_head , Htable_head, file_name) == SUCCESS))
				{
					printf("\033[0;32mDatabase updated with file %s\n\033[0m", file_name);
				}
				else if((update_DB(file_head , Htable_head, file_name) == REPEATED))
				{
					printf("\033[0;31mError: File already present in database.\033[0m\n");
				}
				else
				{
					printf("Could not allocate memory for file \"%s\"", file_name);
				}
				break;
			case 5:
				printf("\033[0;34mEnter the name of the backup file : \033[0m");
				scanf("%s", file_name);

				if((save_DB(Htable_head, file_name) == SUCCESS))
				{
					printf("\033[0;32mSuccess : Database saved into the file %s.\033[0m\n", file_name);
				}
				else
				{
					printf("\033[0;31mError : Could not save database\033[0m\n");
				}
				break;
			case 6:
				printf("\033[0;33m-------Exiting-------\033[0m\n");
				return 0;
			default:
				printf("\033[1;31mEnter valid choice\033[0m\n");
		}
	}
}
