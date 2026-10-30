#include <stdio.h>
#include <string.h>
#include "main.h"

extern FILE *fp;
int open_files( char *file_name , FILE **out_fp )
{
    /*opening file in read binary mode*/
    
    FILE *fp = fopen(file_name , "rb");
    if( fp == NULL )
    {
        printf("ERROR: Unable to open file\n");
        return FAILURE;
    }

    unsigned char header[10];

    /* read the first 10 bytes of the file*/
    if (fread(header, 1, 10, fp) != 10)
    {
        printf("ERROR: Unable to read the file\n");
        fclose(fp);
        return FAILURE;
    }

    /* checking the header is ID3 or not */
    if (header[0] != 'I' || header[1] != 'D' || header[2] != '3')
    {
        printf("ERROR: ID3 tag not found in file\n");
        fclose(fp);
        return FAILURE;
    }
    
    printf("Header: %c%c%c\n", header[0], header[1], header[2]);
    
    /* checking version */
    if (header[3] != 3 || header[4] != 0)
    {
        printf("ERROR: ID3v2.3 version not supported\n");
        fclose(fp);
        return FAILURE;
    }

    printf("ID3v2.3 version verified successfully\n\n");
    /*checking extension */
    if( strcmp(strrchr ( file_name , '.') , ".mp3") != 0  )
    {
        printf("ERROR:Invalid file extension\n");
        fclose(fp);
        return FAILURE;
    }

    unsigned int tag_size;

    /* tag size is in sync-safe integer format 
        every byte MSB is 0 */

    tag_size = ((unsigned int)(header[6] & 0x7F) << 21) |
            ((unsigned int)(header[7] & 0x7F) << 14) |
            ((unsigned int)(header[8] & 0x7F) << 7)  |
            (header[9] & 0x7F);
    *out_fp = fp;
    return tag_size;

    
}