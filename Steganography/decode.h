#ifndef DECODE_H
#define DECODE_H

#include "types.h" 
#include "common.h" 
#include <string.h>

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

/* 
 *  * Structure to store information required for
 *   * decoding stego image to output file
 *    * Info about output and intermediate data is
 *     * also stored
 *      */

typedef struct _DecodeInfo
{
	/*Stego Image Info*/
	char *stego_image_fname;
	FILE *fptr_stego_image;

	/*Output Image Info*/
	char *output_fname;
	FILE *fptr_output;
	int output_file_extn_size;
	char *output_file_extn ;
	int output_file_size;


} DecodeInfo; 

/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/*To perform decoding for image*/
Status do_decoding(DecodeInfo *decInfo);

/*To open files required*/
Status open_decode_files(DecodeInfo *decInfo);

/*Decode magic string*/
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/*To decode*/
Status decode_byte_from_lsb(char data,char *image_buffer);

/*To decode secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/*To decode secret file extension*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/*To decode secret file size*/
Status decode_secret_file_size(DecodeInfo *decInfo);

/*To decode num from image*/
int decode_num_from_img(char *, FILE *);

/*To decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/*to decode data from image*/
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image);


#endif
