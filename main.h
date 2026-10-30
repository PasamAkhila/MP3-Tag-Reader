#ifndef MAIN_H
#define MAIN_H

#define FAILURE         0
#define SUCCESS         1

int open_files(char * file_name,FILE **fp);
int open_edit_files(char *file_name, char *tag, char *new_name);
int open_view_files(char *file_view);
int check_edit_option(char *tag);
int read_and_display_tags(FILE *fp, unsigned int tag_size);
int edit_tag(FILE *fp, unsigned int tag_size,char *tag, char *new_name, char *temp_name);

int copy_bytes(FILE *src, FILE *dest, unsigned long count);

#endif