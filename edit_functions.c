#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "main.h"

#define MAX_ID3_TAG_SIZE 0x0FFFFFFF

/*copying data from source file to destination file */

int copy_bytes(FILE *src, FILE *dest, unsigned long count)
{
    char ch;
    while( count > 0 )
    {
        ch = fgetc(src);
        if( ch == EOF )
            break;
        fputc( ch , dest );
        count--;
    }
    return SUCCESS;
}

int check_edit_option(char *tag)
{
    if (strcmp(tag, "-t") == 0)
        printf("You selected to edit title\n");
    else if (strcmp(tag, "-a") == 0)
        printf("You selected to edit artist\n");
    else if (strcmp(tag, "-A") == 0)
        printf("You selected to edit album\n");
    else if (strcmp(tag, "-y") == 0)
        printf("You selected to edit year\n");
    else if (strcmp(tag, "-c") == 0)
        printf("You selected to edit composer\n");
    else
    {
        printf("ERROR: Invalid edit option\n");
        return FAILURE;
    }

    return SUCCESS;
}

int open_edit_files(char *file_name, char *tag, char *new_name)
{
    FILE *fp;
    FILE *check_fp;
    unsigned char header[10];
    unsigned int tag_size;
    unsigned int temp_name_len;
    char *temp_name;
    int result;

    /* checking the tag option */
    if (check_edit_option(tag) == FAILURE)
        return FAILURE;

    tag_size = open_files(file_name , &fp );
    if(tag_size == FAILURE)
        return FAILURE;
   
    /*creating a temporary file for editing */
    
    temp_name_len = strlen(file_name);
    temp_name = malloc(temp_name_len + 5);

    strcpy(temp_name , file_name);
    strcat(temp_name , ".tmp");

    result = edit_tag(fp , tag_size , tag , new_name , temp_name );
    fclose(fp);
    
    if( result == FAILURE)
    {
        free(temp_name);
        return FAILURE;
    }

    /* rename it with original file name */
    rename(temp_name , file_name);
    printf("MP3 tag updated successfully\n");

    free(temp_name);
    return SUCCESS;
}
int edit_tag(FILE *fp, unsigned int tag_size,char *tag, char *new_name, char *temp_name)
{
    char frame_id[5];
    unsigned int bytes_read = 0;
    unsigned int frame_size, new_frame_size, updated_tag_size;
    unsigned char frame_header[10], new_frame_header[10];
    unsigned char id3_header[10];
    unsigned char *frame_data, *new_frame_data;
    unsigned long remaining_size, audio_start, audio_size;
    long frame_start, remaining_start, file_end;
    unsigned long int text_len;
    FILE *temp_fp;

    if (strcmp(tag, "-t") == 0)
        strcpy(frame_id, "TIT2");
    else if (strcmp(tag, "-a") == 0)
        strcpy(frame_id, "TPE1");
    else if (strcmp(tag, "-A") == 0)
        strcpy(frame_id, "TALB");
    else if (strcmp(tag, "-y") == 0)
        strcpy(frame_id, "TYER");
    else if (strcmp(tag, "-c") == 0)
        strcpy(frame_id, "TCOM");
    else
        return FAILURE;

    while (bytes_read < tag_size)
    {
        /*read the 10 bytes of data and calculate frame size */
        if (fread(frame_header, 1, 10, fp) != 10)
            return FAILURE;

        bytes_read += 10;

        if (frame_header[0] == 0)
            break;

        frame_size = ((unsigned int)frame_header[4] << 24) |
                     ((unsigned int)frame_header[5] << 16) |
                     ((unsigned int)frame_header[6] << 8) |
                     frame_header[7];

        if (frame_size > tag_size - bytes_read)
            return FAILURE;

        /* copying the frame id to frame header of the temporary file */
        if (memcmp(frame_header, frame_id, 4) != 0)
        {
            fseek(fp, frame_size, SEEK_CUR);
            bytes_read += frame_size;
            continue;
        }

        /*frame starting position */
        frame_start = ftell(fp) - 10;

        frame_data = malloc(frame_size);
        if (frame_data == NULL)
            return FAILURE;

        /*reading the old frame data */
        if (fread(frame_data, 1, frame_size, fp) != frame_size)
        {
            free(frame_data);
            return FAILURE;
        }

        if (frame_size == 0 || frame_data[0] != 0)
        {
            free(frame_data);
            printf("ERROR: Unsupported text encoding\n");
            return FAILURE;
        }

        /*calculating new frame size and creating new frame data */
        text_len = strlen(new_name);
        new_frame_size = text_len + 1;
        updated_tag_size = tag_size - frame_size + new_frame_size;

        new_frame_data = malloc(new_frame_size);
        if (new_frame_data == NULL)
        {
            free(frame_data);
            return FAILURE;
        }

        /* copying the new frame data and new frame header */
        new_frame_data[0] = 0;
        memcpy(new_frame_data + 1, new_name, text_len);
        memcpy(new_frame_header, frame_header, 10);

        new_frame_header[4] = (new_frame_size >> 24) & 0xFF;
        new_frame_header[5] = (new_frame_size >> 16) & 0xFF;
        new_frame_header[6] = (new_frame_size >> 8) & 0xFF;
        new_frame_header[7] = new_frame_size & 0xFF;

        temp_fp = fopen(temp_name, "wb");
        
        if (temp_fp == NULL)
        {
            free(frame_data);
            free(new_frame_data);
            return FAILURE;
        }

        rewind(fp);
        fread(id3_header, 1, 10, fp);
        fwrite(id3_header, 1, 10, temp_fp);

        /* Copying data before selected frame */
        fseek(fp, 10, SEEK_SET);
        copy_bytes(fp, temp_fp, frame_start - 10);

        /* Writing
         updated frame */
        fwrite(new_frame_header, 1, 10, temp_fp);
        fwrite(new_frame_data, 1, new_frame_size, temp_fp);

        /* Copy remaining ID3 tag data */
        remaining_start = frame_start + 10 + frame_size;
        remaining_size = tag_size - (frame_start - 10) - 10 - frame_size;

        fseek(fp, remaining_start, SEEK_SET);
        copy_bytes(fp, temp_fp, remaining_size);

        /* Copy MP3 audio data */
        audio_start = 10 + tag_size;

        fseek(fp, 0, SEEK_END);
        file_end = ftell(fp);
        audio_size = file_end - audio_start;

        fseek(fp, audio_start, SEEK_SET);
        copy_bytes(fp, temp_fp, audio_size);

        /* Update ID3 tag size */
        id3_header[6] = (updated_tag_size >> 21) & 0x7F;
        id3_header[7] = (updated_tag_size >> 14) & 0x7F;
        id3_header[8] = (updated_tag_size >> 7) & 0x7F;
        id3_header[9] = updated_tag_size & 0x7F;

        fseek(temp_fp, 0, SEEK_SET);
        fwrite(id3_header, 1, 10, temp_fp);

        fclose(temp_fp);
        free(frame_data);
        free(new_frame_data);

        printf("Tag updated successfully\n");
        return SUCCESS;
    }

    printf("ERROR: Selected frame not found\n");
    return FAILURE;
}
