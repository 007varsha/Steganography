#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <stdlib.h>

char *exten;

/* Function Definitions */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)                     //Function defination to check if all the passed arguments are in a proper manner
{
    if(strstr(argv[2],".bmp"))                                                              //Condition to check if the 2nd argument in the CLA is a .bmp file
    {
       encInfo->src_image_fname=argv[2];                                                    //If yes, then storing it in a structure variable
    }
    else
    {
        return e_failure;
    }
    if(exten=strstr(argv[3],"."))                                                          //Condition to check if the 3rd argument in the CLA is a file or not
    {
        encInfo->secret_fname=argv[3];                                                     //If yes, then storing it in a structure variable
    }
    
    else
    {
        return e_failure;
    }
    if(argv[4]==NULL)                                                                      //Condition to check if the 4th argument is passed or not
    {
        encInfo->stego_image_fname="default.bmp";                                          //If not then storing the contents in a default.bmp file
    }
    else if(strstr(argv[4],".bmp"))                                                        //If a name is provided then storing it in that file
    {
        encInfo->stego_image_fname=argv[4];
    }
    else
    {
        return e_failure;
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)                                                   //Function defination for encoding
{
    printf("INFO : Opening required files\n");
    int ret= open_files(encInfo);                                                         //Function call to open the files
    if(ret == e_success)
    {
        printf("INFO : Opened %s\n",encInfo->src_image_fname);
        printf("INFO : Opened %s\n",encInfo->secret_fname);
        printf("INFO : Opened %s\nINFO : Done.\n",encInfo->stego_image_fname);

    }
    else
        return e_failure;
    printf("\n## Encoding procedure started ##\n\nINFO : Checkinng capacity...\n");
    ret = check_capacity(encInfo);                                                        //Function call to check if the secret file size is less that the carrier file
    if (ret== e_failure)
    {
        printf("INFO : Capacity not sufficient\n");
    }
    else if (ret == e_success)
    {
        printf("INFO : Capacity is sufficient\n");
    }
    printf("INFO : Copying header file first...\n");
    copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image);                  //Function call to copy the header files of the sorce file to the  destiation file 
    printf("INFO : Copying MAGIC STRING...\n");
    encode_magic_size(MAGIC_STRING,encInfo);                                             //Function call to encode the magic string size
    encode_magic_string(MAGIC_STRING,encInfo);                                           //Function call to encode the magic string
    printf("INFO : Copying Secret message...\n");
    encode_secret_file_extn_size(strlen(exten), encInfo);                                //Function call to encode the secretfile extension file size
    encode_secret_file_extn(exten,encInfo);                                              //Function call to encode the  secret file extension
    encode_secret_file_size(encInfo->size_secret_file,encInfo);                          //Function call to encode the secret file size
    encode_secret_file_data(encInfo);                                                    //Function call to encode the contents of the secret file
    printf("INFO : Copying Remaining message...\n");
    copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image);         //Function call to encode rest of the data into the output file
    printf("INFO : Copying remaining data...\n");

    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)                                              //Function defination to check the file capacity
{
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);            //Function call to get the size of the .bmp file and storing it in a structure variable 
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);                      //Function call to get the size of the secret file and storing it in a structure variable 
    strcpy(encInfo->extn_secret_file,".txt");                                           //Storing the exension of the secret file in a structure variable
    int secret_extn_size=strlen(encInfo->extn_secret_file);                             //Storing the  size of the secret file extension
    int magic_str_len=strlen(MAGIC_STRING);                                             //Storing the size of the magic string
    if(encInfo->image_capacity > (54 + (magic_str_len)*8 +(secret_extn_size)*8 +sizeof(secret_extn_size)*8 +sizeof(encInfo->size_secret_file)*8 +(encInfo->size_secret_file)*8))        //Checking the image size is higher than the sceret file size
        return e_success;
    else
        return e_failure;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)                         //Function dfination to encode rest of the image  data into the output file
{
    char ch;
    while (fread(&ch,1,1,fptr_src))
    {
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_encode;
}


Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)                     //Function defination to copy the header file of the source image to the output file
{
    rewind(fptr_src_image);
    char buffer[54];
    fread(buffer, 54, 1,fptr_src_image);
    fwrite(buffer, 54, 1,fptr_dest_image);
}

uint get_file_size(FILE *fptr)                                                       //Function defination to get the file size
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

Status encode_magic_size(const char *magic_string,EncodeInfo *encInfo)              //Function defination to encode the magicstring size
{
   char arr[32];
   fread(arr, 32, 1,encInfo->fptr_src_image);
   encode_size_to_lsb(strlen(magic_string),arr);
   fwrite(arr, 32, 1,encInfo->fptr_stego_image);
   return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)           //Function defination to encode the magic string
{
    int len=strlen(MAGIC_STRING);
    char arr[8];
    for(int i=0;i<len;i++)
    {
        fread(arr,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i],arr);
        fwrite(arr,8,1,encInfo->fptr_stego_image);
    }
}


Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)                  //Function defination to store the secret file extension size
{
   char arr[32];
   fread(arr,32,1,encInfo->fptr_src_image);
   encode_size_to_lsb(size,arr);
   fwrite(arr,32,1,encInfo->fptr_stego_image);
   return e_success;
}


Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)        //Function defination to encode the secret file extension
{
    char arr[8];
    for(int i=0;i<strlen(file_extn);i++)
    {
        fread(arr,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(file_extn[i],arr);
        fwrite(arr,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)                //Function defination to encode the secret file size
{
  char arr[64];
  fread(arr,64,1,encInfo->fptr_src_image);
  encode_size_to_lsb(file_size,arr);
  fwrite(arr,64,1,encInfo->fptr_stego_image);
  return e_success;   
}


Status encode_secret_file_data(EncodeInfo *encInfo)                                //Function defination to enode the secret file data 
{
   
   rewind(encInfo->fptr_secret);
   char arr[8],ch;
    for(int i=0;i<encInfo->size_secret_file;i++)
    {
        fread(&ch,1,1,encInfo->fptr_secret);
        fread(arr,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(ch,arr);
        fwrite(arr,8,1,encInfo->fptr_stego_image);
    }
   
}


/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}


Status encode_byte_to_lsb(char data, char *image_buffer)                                //Function defination to encode the lsb of the file data with the size of the secret data and extensions
{
    for(int i=7;i>=0;i--)
    {
        if(data&(1<<i))
        {
            image_buffer[7-i]=image_buffer[7-i] |1;

        }
        else
        {
            image_buffer[7-i]=image_buffer[7-i] & ~1;
        }
    }
    return e_success;
}

Status encode_size_to_lsb(int data,char *image_buffer)                                 //Function defination to encode the lsb of the file data with the bytes of the secret data
{
    int val=0;
    for(int i=31;i>=0;i--)
    {
        val=((((unsigned)data>>i))&1);
        image_buffer[31-i]=(val|(image_buffer[31-i]&(~1)));
    }
    return e_success;
}


/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
