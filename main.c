#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])                                                //Main function
{
    int ret = check_operation_type(argv);                                       //Function call to check weather to perform encoding or decoding operation
    if(ret == e_encode)                                                         
    {
        printf("Select encoding\n");
        EncodeInfo encinfo;                                                     //Creating a structure variable
        if(read_and_validate_encode_args(argv,&encinfo) == e_success)           //Function call to check if all the passed arguments in the CLA are proper or not
        {
            do_encoding(&encinfo);                                              //Function call to perform encoding operation
            printf("Success\n");                                                
        }
        else
        {
            printf("Failure\n");                                                //Message if the encoding  operion encouters any errors
        }
    }
    else if(ret == e_decode)                                                    
    {
        printf("Select decoding\n");
        DecodeInfo decodeinfo;                                                  //Declaring a structure variable
        if((read_and_validate_decode_args(argv,&decodeinfo)==e_failure))        //Function call to check if all the passed arguments in the CLA are proper or not
        {
            printf("Decoding Validation part error\n");
        }
        else
        {
            do_decoding(&decodeinfo);                                           //Function call to perform decoding opertion
        }
    }
    else
    {
        printf("Error\n");
    }

}

OperationType check_operation_type( char *argv[])                               //Function defination to check if the 1st argument is to perform encoding or decoding
{
    if(strcmp(argv[1],"-e")==0)                                                 //If it is encoding, then returning 0
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)                                            //If it is decoding, then returning 1
    {
        return e_decode;
    }
    else                                                                        //If it is neither of those,then return 2
    {
        return e_unsupported;
    }
}
