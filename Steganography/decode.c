/* DECODING FILE */

#include "common.h"
#include "types.h"
#include <stdio.h>
#include <string.h>
#include "decode.h"
#include <stdlib.h>


/* To read and verify CLAs */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	if(strcmp((strstr(argv[2],".bmp")), ".bmp") == 0)
	{
		decInfo->stego_image_fname = argv[2];
	}
	else
	{
		return d_failure;
	}

	if(argv[3] != NULL)
	{
		decInfo->output_fname = argv[3];
	}
	else
        {
         	decInfo->output_fname = "decoded.txt";
    	}	    
	
	return d_success;
}

/* To open files for decoding */
Status open_decode_files(DecodeInfo *decInfo)
{

	decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");

	if (decInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

		return d_failure;
	}

	decInfo->fptr_output = fopen(decInfo->output_fname, "w");

	if (decInfo->fptr_output == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);

		return e_failure;
	}

	return d_success;
}

/* To decode data from image */
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image)
{
	char image_buffer[8];
	for(int i = 0; i < size-1; i++)
	{
		fread(image_buffer,1,8,fptr_stego_image);
		data[i] = decode_byte_from_lsb((*data), image_buffer);
	}
	data[size-1] = '\0';

	return d_success;
}

/* To decode byte from lsb */
Status decode_byte_from_lsb(char data, char *image_buffer)
{
	data = 0x00;
	unsigned char mask = 0x80;
	for(int i = 0; i < 8; i++)
	{
		if(image_buffer[i] & 1)
		{
			data = data | mask;
		}
		mask = mask >> 1;
	}
	return data;
}

/*To decode magic string */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{

	int size = (strlen(magic_string) + 1);
	char data[size];
	fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
	decode_data_from_image(data, size , decInfo->fptr_stego_image);
	if(strcmp(data,magic_string) == 0)
	{
		return d_success;
	}
	return d_failure;
}

/* To decode num from pointer */
int decode_num_from_img(char *image_buffer, FILE *fptr)
{
	int size = 0;
	for (int i = 0; i < 32; i++)
	{
		if (image_buffer[i] & 1)
		size = size | 1 << (31 - i);
	}
	return size;
}

/* To decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
	char image_buffer[32] = {};
	fread(image_buffer, 1, 32, decInfo -> fptr_stego_image);
	decInfo->output_file_extn_size = decode_num_from_img(image_buffer, decInfo->fptr_stego_image);
	return d_success;

}

/* To decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
	decInfo->output_file_extn = malloc(1 * ((decInfo->output_file_extn_size) + 1));

	decode_data_from_image(decInfo->output_file_extn, decInfo -> output_file_extn_size + 1, decInfo->fptr_stego_image);

	return d_success;
}


/* To decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
	char image_buffer[32] = {};
	fread(image_buffer, 1, 32, decInfo -> fptr_stego_image);
	decInfo -> output_file_size = decode_num_from_img(image_buffer, decInfo -> fptr_stego_image);

	return d_success;
}

/* To decode the secret file data from stego */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
	char image_buffer[8] = {};

	for (int i = 0; i < decInfo -> output_file_size; i++)
	{
		fread(image_buffer, 1, 8, decInfo -> fptr_stego_image);
		char data = decode_byte_from_lsb(data,image_buffer);
		fwrite(&data, 1, 1, decInfo -> fptr_output);
	}
	return d_success;
}

/* To do decoding by calling individual functions sequentially */
Status do_decoding(DecodeInfo *decInfo)
{
	if(open_decode_files(decInfo) == d_success)
	{
		printf("Success\t:\tOpen Files\n");
		
		if(decode_magic_string(MAGIC_STRING,decInfo) == d_success)
		{
			printf("Success\t:\tDecode Magic String\n");
			
			if (decode_secret_file_extn_size(decInfo) == d_success)
			{
				printf("Success\t:\tDecode File Extension Size\n");
				
				if(decode_secret_file_extn(decInfo) == d_success)
				{
					printf("Success\t:\tDecode Secret File Extension\n");
					
					if (decode_secret_file_size(decInfo) == d_success)
					{
						printf("Success\t:\tDecode Secret File Size\n");
						
						if (decode_secret_file_data(decInfo) == d_success)
						{
							printf("Success\t:\tDecode Secret File Data\n");
							return d_success;
						}
						else
						{
							printf("Failure\t:\tDecode Secret File Data\n");
							return d_failure;
						}
					}
					else
					{
						printf("Failure\t:\tDecode Secret File Size\n");
						return d_failure;
					}
				}
				else
				{
					printf("Failure\t:\tDecode Secret File Extension\n");
					return d_failure;
				}
			}
			else
			{
				printf("Failure\t:\tDecode File Extension Size\n");
				return d_failure;
			}
		}
		else
		{
			printf("Failure\t:\tDecode Magic String\n");
		}
	}
	else
	{
		printf("Failure\t:\tOpen Files\n");
	}

	return d_success;
}
