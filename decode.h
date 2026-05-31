#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include "common.h"

typedef struct _DecodeInfo
{
    char *src_image_fname;
    FILE *fptr_src_image;
    char  secret_fname[200];
    FILE *fptr_secret_file;
    long secret_data;
    char *extn_secret_file;
    int  secret_file_extn_size;
    int  file_size;
} DecodeInfo;

//Function prototype for reading and validating the command line arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decodeinfo);

//Function prototype to open the encoded file
Status file_open(DecodeInfo *decodeinfo);

//Function prototype to decode the encoded file
Status do_decoding(DecodeInfo *decodeinfo);

//Function prototype to decoded the magic string size
Status decode_magic_size(FILE *fptr_src_image, DecodeInfo *decodeinfo);

//Function prototype to decode the magic string
Status decode_magic_string(FILE *fptr_src_image,int magic_size);

//Function prototype to decode the data from lsb 
Status decode_byte_from_lsb(char *arr,char *buffer );

//Function rototype to decode the secret file extension size
Status decode_secret_file_extn_size(int size, DecodeInfo *decodeinfo);

//Function prototype to decode the data from lsb 
Status decode_size_from_lsb(char *arr, int *size);

//Function prototype to decode the secret file extension
Status decode_secret_file_extn(FILE *fptr_src_image,char*extn_secret_file,DecodeInfo *decodeinfo, int ext_size);

//Function prototype to decode the secret file size
Status decode_secret_file_size(FILE *fptr_src_image, DecodeInfo *decodeinfo);

//Function prototype to decode  the secret data
char* decode_secret_data(FILE *fptr_src_image, DecodeInfo *decodeinfo,int sec_size);

//Function prototype to recreate the file
Status copy_to_secret_file(DecodeInfo *decodeinfo, char *buffer);

#endif
