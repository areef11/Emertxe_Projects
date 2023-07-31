/*
 * Name			: Areef Ahmad
 * Date			: 14-01-2023
 * Description		: C-Project : LSB Steganography 
 */

#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

/* Main function to check CLAs passed and proceed with encoding or decoding*/
int main(int argc, char *argv[])
{
	if (check_operation_type(argv) == e_encode)					//for encoding
	{
		printf("Choosen Encoding\n");
		EncodeInfo encInfo ; 
		if (read_and_validate_encode_args(argv, &encInfo) == e_success)
		{
			printf("Success\t:\tRead and validate\n");
			printf("Started Encoding...\n");
			if(do_encoding(&encInfo) == e_success)
			{
				printf("Success\t:\tDo Encoding\n");
				printf("<---- DATA ENCODED SUCCESSFULLY ---->\n");
			}
			else
			{
				printf("Failed\t:\tDo Encoding\n");
				return 1;
			}
		}
		else
		{
			printf("Failed\t:\tRead and validate\n");
			return 1;
		}
	}
	else if(check_operation_type(argv) == e_decode)					//for decoding
	{
		printf("Choosen Decoding\n");
		DecodeInfo decInfo;
		if (read_and_validate_decode_args(argv, &decInfo) == d_success)
		{
			printf("Success\t:\tRead and validate\n");
			printf("Started Decoding...\n");
			if(do_decoding(&decInfo) == d_success)
			{
				printf("Success\t:\tDo Decoding\n");
				printf("<---- DATA DECODED SUCCESSFULLY ---->\n");
			}
			else
			{
				printf("Failed\t:\tDo Dncoding\n");
			}
		}
		else
		{
			printf("Failed\t:\tRead and validate\n");
			return -1;
		}
	}
	else										//in case of unsuitable CLA input
	{
		printf("Please pass the inputs like.....\nEncoding:./a.out -e <filename.bmp> <filename.txt>\nDecoding:./a.out -d <filename.bmp>\n");
		return 1;
	}

	return 0;
}

OperationType check_operation_type(char *argv[])					//to check type of operation to be performed
{
	if(strcmp(argv[1],"-e") == 0)
	{
		return e_encode;
	}
	else if(strcmp(argv[1],"-d") == 0)
	{
		return e_decode;
	}
	else
	{
		return e_unsupported;
	}
}
