/*Encoding File*/

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)				//to check CLAs
{
	if(strcmp(strstr(argv[2],".bmp") ,".bmp") == 0)
	{
		encInfo->src_image_fname = argv[2];
	}
	else
	{
		return e_failure;
	}
	if(strcmp(strstr(argv[3],".txt"), ".txt") == 0)
	{
		encInfo->secret_fname = argv[3];
	}
	else
	{
		return e_failure;
	}
	if(argv[4] != NULL)
	{
		encInfo->stego_image_fname = argv[4];
	}
	else
	{
		encInfo->stego_image_fname = "stego.bmp";
	}

	return e_success;
}


/*Function to calculate the image size of bmp file*/
uint get_image_size_for_bmp(FILE *fptr_image)
{
	uint width, height;
	fseek(fptr_image, 18, SEEK_SET);

	fread(&width, sizeof(int), 1, fptr_image);
	printf("Width = %u\n", width);

	fread(&height, sizeof(int), 1, fptr_image);
	printf("Height = %u\n", height);

	return width * height * 3;
}


/* 
 *  * Get File pointers for i/p and o/p files
 *   * Inputs: Src Image file, Secret file and
 *    * Stego Image file
 *     * Output: FILE pointer for above files
 *      * Return Value: e_success or e_failure, on file errors
 *       */
Status open_files(EncodeInfo *encInfo)
{
	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
	
	//error handling
	if(encInfo->fptr_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

		return e_failure;
	}

	encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
	
	//error handling
	if(encInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

		return e_failure; 
	}

	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");

	//error handling
	if(encInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

		return e_failure;
	}

	return e_success;
}


/*To check if src image is large enough to be encoded with secret data*/
Status check_capacity(EncodeInfo *encInfo)
{
	encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
	encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

	if(encInfo-> image_capacity > 16 + 32 + 32 + 32 + 8 * (get_file_size(encInfo->fptr_secret)))
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}


/*To calculate file size*/
uint get_file_size(FILE *fptr)
{
	fseek(fptr, 0, SEEK_END);
	return ftell(fptr);
}

/*Function to copy bmp file header*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	char str[54];
	fseek(fptr_src_image, 0, SEEK_SET);
	fread(str, sizeof(char), 54, fptr_src_image);
	fwrite(str, sizeof(char), 54, fptr_dest_image);
	return e_success;
}


/*Encode magic string function*/
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
	encode_data_to_image(magic_string, strlen(magic_string), encInfo-> fptr_src_image, encInfo->fptr_stego_image, encInfo);
	return e_success;
}

/*To encode data into the output image*/
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image, EncodeInfo *encInfo)
{
	int i;
	for(i = 0; i < size; i++)
	{
		fread(encInfo->image_data, sizeof(char), 8, fptr_src_image);
		encode_byte_to_lsb(data[i], encInfo->image_data);
		fwrite (encInfo->image_data, sizeof(char), 8, fptr_stego_image);
	}
	return e_success;
}

/*To encode bytes from secret file to lsb bits stego file*/
Status encode_byte_to_lsb(char data, char *image_buffer)
{
	unsigned int mask = 1 << 7;
	int i;
	for (i = 0; i < 8; i++)
	{
		image_buffer[i] = ((image_buffer[i] & 0xFE) | (data & mask) >> (7 - i));
		mask = mask >> 1;
	}

	return e_success;

}

/*To encode secret file extension size*/
Status encode_secret_file_extn_size (int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
	char str[32];
	fread(str, 32, 1, fptr_src_image);
	encode_size_to_lsb(size, str);
	fwrite(str, 32, 1, fptr_stego_image);
	return e_success;
}

/*To encode size data*/
Status encode_size_to_lsb(int size, char *image_buffer)
{
	unsigned int mask = 1 << 31;
	int i;
	for(i = 0; i < 32; i++)
	{
		image_buffer[i] = ((image_buffer[i] & 0xFE ) | (size & mask) >> (31 - i));
		mask = mask >> 1;
	}

	return e_success;
}

/*To encode secret file extension*/
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
	file_extn = ".txt";

	encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image, encInfo);

	return e_success;
}

/*To encode secret file size*/
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
	char str[32];
	fread(str, 32, 1, encInfo->fptr_src_image);
	encode_size_to_lsb(file_size, str);
	fwrite(str, 32, 1, encInfo->fptr_stego_image);
	return e_success;
}

/*To encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
	char ch;
	int i;

	fseek(encInfo->fptr_secret, 0, SEEK_SET);

	for(i = 0; i < encInfo->size_secret_file; i++)
	{
		fread(&ch, 1, 1, encInfo->fptr_secret);
		fread(encInfo->image_data, 8, 1, encInfo->fptr_src_image);
		encode_byte_to_lsb(ch, encInfo->image_data);
		fwrite(encInfo->image_data, 8, 1, encInfo->fptr_stego_image);
	}
	return e_success;
}

/*To copy remaining data to get copy of source image*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	char ch;
	while((fread (&ch, 1, 1, fptr_src)) > 0)
	{
		fwrite(&ch, 1, 1, fptr_dest);
	}
	return e_success;
}

/*To perform decoding by calling individual functions*/
Status do_encoding(EncodeInfo *encInfo)
{
	if (open_files(encInfo) == e_success)
	{
		printf("Success\t:\tOpen Files");
		if(check_capacity(encInfo) == e_success)
		{
			printf("Success\t:\tCheck Capacity\n");
			if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
			{
				printf("Success\t:\tCopy bmp Header\n");
				if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
				{
					printf("Success\t:\tEncode Magic String\n");
					strcpy(encInfo->extn_secret_file, (strstr(encInfo->secret_fname, ".txt")));
					if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
					{
						printf("Success\t:\tEncode Secret File Extn Size\n");
						if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
						{
							printf("Success\t:\tEncode Secret File Extension\n");
							if(encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
							{
								printf("Success\t:\tEncode Secret File Size\n");
								if(encode_secret_file_data(encInfo) == e_success)
								{
									printf("Success\t:\tEncode Secret File Data\n"); 
									if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
									{
										printf("Success\t:\tCopy Remaining Data\n");
									}
									else
									{
										printf("Failure\t:\tCopy Remaining Data\n");
										return e_failure;
									}
								}
								else
								{
									printf("Failure\t:\tEncode Secret File Data");
								}
							}
							else
							{
								printf("Failure\t:\tEncoding Secret File Size\n");
								return e_failure;
							}
						}
						else
						{
							printf("Failure\t:\tEncoding Secret File Extension\n");
							return e_failure;
						}
					}
					else
					{
						printf("Failure\t:\tEncoding Secret File Extension Size\n");
						return e_failure;
					}
				}
				else
				{
					printf("Failure\t:\tEncoding Magic String\n");
					return e_failure;
				}
			}
			else
			{
				printf("Failure\t:\tCopy bmp Header\n");
				return e_failure;
			}
		}
		else
		{
			printf("Failure\t:\tCheck Capacity\n");
			return e_failure;
		}
	}
	else	
	{
		printf("Failure\t:\tOpen File\n");
		return e_failure;
	}	

	return e_success;

}
