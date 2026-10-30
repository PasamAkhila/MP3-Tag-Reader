#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

FILE *fp;
int open_view_files(char *file_view)
{
    unsigned tag_size;
    FILE *fp;
    
    /* opening files and recieving the tagsize */
    
    tag_size = open_files(file_view ,&fp);
    if(tag_size == FAILURE)
        return FAILURE;

    if(read_and_display_tags(fp,tag_size) == FAILURE)
    {
        printf("ERROR: Unable to display the details\n");
        return FAILURE;
    }
    return SUCCESS;
}

int read_and_display_tags(FILE *fp,unsigned int tag_size)
{
    char frame_header[10];
    unsigned int bytes_read = 0;
    while(bytes_read < tag_size)
    {
        /* read next 10 bytes from the file */
        if(fread(frame_header , 1 , 10 , fp) != 10)
        {
            break;
        }
        unsigned int frame_size;

        /*calculating frame size */

        frame_size =((unsigned int)(unsigned char)frame_header[4] << 24) |
                    ((unsigned int)(unsigned char)frame_header[5] << 16) |
                    ((unsigned int)(unsigned char)frame_header[6] << 8)  |
                    (unsigned char)frame_header[7];

        if (frame_size > tag_size - bytes_read - 10)
        {
            printf("ERROR: Invalid frame size\n");
            return FAILURE;
        }

        /* copying the data to the string from the file */
        
        char *str = malloc(frame_size);
        if( str == NULL)
        {
            printf("Memory allocation failed\n");
            return FAILURE;
        }
        if (fread(str, 1, frame_size, fp) != frame_size)
        {
            printf("ERROR: Unable to read frame data\n");
            free(str);
            return FAILURE;
        }

        /*comparing with the frame ids and printing the data */
        
        if (memcmp(frame_header, "TIT2", 4) == 0)
        {
            printf("Frame ID: %.4s\n", frame_header);
            
            if (frame_size >= 2 && str[0] == 0)
            {
                printf("Title: %s\n\n", str + 1);
            }
            else if( str[0] == 1)
                printf("Title: %s\n\n", str + 3 );
        }
        else if (memcmp(frame_header, "TPE1", 4) == 0)
        {
            printf("Frame ID: %.4s\n", frame_header);

            if (frame_size >= 2 && str[0] == 0)
            {
                printf("Artist: %s\n\n", str + 1);
            }
            else if( str[0] == 1)
                printf("Artist: %s\n\n", str + 3);
        }
        else if (memcmp(frame_header, "TALB", 4) == 0)
        {
            printf("Frame ID: %.4s\n", frame_header);

            if (frame_size >= 2 && str[0] == 0)
            {
                printf("Album: %s\n\n", str + 1);
            }
            else if( str[0] == 1)
                printf("Album: %s\n\n", str + 3);
        }
        
        else if (memcmp(frame_header, "TYER", 4) == 0)
        {
            printf("Frame ID: %.4s\n", frame_header);

            if (frame_size >= 2 && str[0] == 0)
            {
                printf("Year: %s\n\n", str + 1);
            }
            else if( str[0] == 1)
                printf("Year: %s\n\n", str + 3);
            
        }
        
        else if (memcmp(frame_header, "TCON", 4) == 0)
        {
            printf("Frame ID: %.4s\n", frame_header);
            
            if (frame_size >= 2 && str[0] == 0)
            {
                printf("Content: %s\n\n", str + 1);
            }
            else if( str[0] == 1)
                printf("Content: %s\n\n", str);
            
        }
        else if (memcmp(frame_header, "TCOM", 4) == 0)
        {
            printf("Frame ID: %.4s\n", frame_header);
            
            if (frame_size >= 2 && str[0] == 0)
            {
                printf("Composer: %s\n\n", str + 1);
            }
            else if( str[0] == 1)
                printf("Composer: %s\n\n", str);
            
        }
        free(str);
        
        /*updating bytes_read for checking loop */
        bytes_read += 10 + frame_size;
    }
    return SUCCESS;
}