#include <stdio.h>
#include "decode.h"
#include <string.h>
#include "types.h"
#include "common.h"


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decodeinfo)
{
    if(strstr(argv[2],".bmp"))
    {
        //strcpy(decodeinfo->secret_fname, argv[3]);
        decodeinfo->src_image_fname=argv[2];
    }
    else
    {
        return e_failure;
    }
    if(argv[3]!=NULL)
    {
        strcpy(decodeinfo->secret_fname, argv[3]);
        //decodeinfo->secret_fname=argv[3];
    }
    else
    {
         strcpy(decodeinfo->secret_fname, "default");
         //decodeinfo->secret_fname="default";
    }
    return e_success;
}
Status do_decoding(DecodeInfo *decodeinfo)                                                                     //Function defination to decode the encoded file
{
    int magic_size=0;
    if(file_open(decodeinfo)==e_success)                                                                       //Opening the encoded file
    {
        printf("File opened\n");
    }
    else
    {
        printf("Error in opening the file\n");
    }
    int ext_size;
    magic_size=decode_magic_size(decodeinfo->fptr_src_image, decodeinfo);                                      //Function call to decode the magic string  size and store it in a variable
    if(decode_magic_string(decodeinfo->fptr_src_image,magic_size)==e_success)                                  //Function call to dcode the magic string
    {
        ext_size=decode_secret_file_extn_size((4*8),decodeinfo);                                               //Decoding the secret file extension size and storing it in a variable
    }
    else
    {
        return e_failure;
    }
    decode_secret_file_extn(decodeinfo->fptr_src_image,decodeinfo->extn_secret_file,decodeinfo,ext_size);     //Function call to decode the secret file extension
    int sec_size = decode_secret_file_size(decodeinfo->fptr_src_image, decodeinfo);                           //Function call to decode the secre file size  nd storing it in a variable  
    char buffer[sec_size];
    strcpy(buffer,decode_secret_data(decodeinfo->fptr_src_image, decodeinfo,sec_size));                       //Function call to decode the secret data and storing it in a variable
    copy_to_secret_file(decodeinfo,buffer);                                                                   //Fucnction call to retrive the secret file form the encoded file
}

Status decode_magic_size(FILE *fptr_scr_image, DecodeInfo *decodeinfo)                                        //Function defination to decode the magic sting size
{
    char arr[32];
    fread(arr,32,1,fptr_scr_image);
    decode_size_from_lsb(arr,&decodeinfo->file_size);
    printf("INFO : Decoding  magic string size...\n");
    return decodeinfo->file_size;
}

Status decode_magic_string(FILE *fptr_src_image, int magic_size)                                               //Function defination to decode the magic string 
{
    char key[100];
    printf("Enter the Magic string : ");
    scanf("%s",key);
    char arr[8],buffer[magic_size];
    int i;
    for(i=0;i<magic_size;i++)
    {
        fread(arr,8,1,fptr_src_image);
        decode_byte_from_lsb(arr,&buffer[i]);                                                                 //Function call to  decode the message from the lsb of the encoded file
    }
    buffer[i]='\0';
    if(strcmp(key,buffer)==0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status decode_secret_file_extn(FILE *fptr_src_image,char *extn_secret_file,DecodeInfo *decodeinfo, int ext_size)  //Function defination to decode the secret file extension
{
    char arr[8],arr1[ext_size];
    int i;
    for(i=0;i<ext_size;i++)
    {
        fread(arr,8,1,fptr_src_image);
        decode_byte_from_lsb(arr,&arr1[i]);                                                                       //Function call to  decode the message from the lsb of the encoded file
    }
    arr1[i]='\0';
    printf("INFO : Decoding extension...\n");
    printf("INFO : Decoding file name...\n");
    strcat(decodeinfo->secret_fname,arr1);
    printf("INFO : Concatinating file name with extension...\n");

}
char* decode_secret_data(FILE *fptr_src_image, DecodeInfo *decodeinfo,int sec_size)                              //Function defination to decode the secret data
{
    char arr[8];
    static char buffer[500];
    int i;
    for(i=0;i<=sec_size-1;i++)
    {
        fread(arr,8,1,fptr_src_image);
        decode_byte_from_lsb(arr,&buffer[i]);                                                                   //Function call to  decode the message from the lsb of the encoded file
    }
    buffer[i]='\0';
    printf("INFO : Decoding secret data...\nSuccess\n");
    return buffer;
}
Status decode_byte_from_lsb(char *arr,char *buffer)                                                            //Function defination to decode the message from the lsb of the encoded files
{
    char ch=0,n;
    for(int i=0;i<8;i++)
    {
        n=arr[i]&1;
        ch=n<<(7-i)|ch;
    }
    *buffer=ch;
}
Status copy_to_secret_file(DecodeInfo *decodeinfo, char *buffer)                                              //Function defination to write the encoded data into the output file
{
    FILE *fptr=fopen(decodeinfo->secret_fname,"w+");
    fprintf(fptr,"%s",buffer);
    if(fptr==NULL)
    {
        printf("Unable to open the file\n");
    }
}



Status decode_secret_file_extn_size(int size, DecodeInfo *decodeinfo)                                         //Function defination to decode the secret file extension size
{
    char arr[size];
    fread(arr,size,1,decodeinfo->fptr_src_image);
    decode_size_from_lsb(arr,&decodeinfo->secret_file_extn_size);                                             //Function call to decode the size from the encoded file
    printf("INFO : Decoding secert file extension size...\n");
    return decodeinfo->secret_file_extn_size;
}

Status decode_secret_file_size(FILE *fptr_src_image, DecodeInfo *decodeinfo)                                  //Functin defination to decode the secret file size from the encoded file
{
    char arr[64];
    fread(arr,64,1,fptr_src_image);
    decode_size_from_lsb(arr,&decodeinfo->file_size);                                                         //Function call to decode the size from the encoded file
    printf("INFO : Decoding secret data file size...\n");
    return decodeinfo->file_size;
}

Status decode_size_from_lsb(char *arr,int *num)                                                               //Function defination to decode the size from the  encoded file
{
    int size=0;
    for(int i=0;i<32;i++)
    {
        int num1=arr[i]&1;
        size=num1<<(31-i)|size;
    }
    *num=size;
}
Status file_open(DecodeInfo *decodeinfo)                                                                    //Function defination to open the encoded file
{
    decodeinfo->fptr_src_image = fopen(decodeinfo->src_image_fname,"r");
    if(decodeinfo->fptr_src_image==NULL)
    {
        perror("fopen");
        fprintf(stderr,"ERROR: unable to open the file %s\n",decodeinfo->src_image_fname);
        return e_failure;
    }
    fseek(decodeinfo->fptr_src_image, 54, SEEK_SET);                                                        //Function to make the file pointer skip the header files of the encoded file
    return e_success;
}
