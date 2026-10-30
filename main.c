#include <stdio.h>
#include <string.h>
#include "main.h"

int main( int argc , char *argv[])
{
    if( argc < 2 )
    {
        printf("Please provide valid arguments\nfor help please give \"./a.out -help\" \n");
        return FAILURE;
    }
    
    if (strcmp(argv[1], "-v") == 0)    {
        if(open_view_files(argv[2])== FAILURE)
            return FAILURE;
    }
    
    else if (strcmp(argv[1], "-e") == 0)
    {
        if( open_edit_files(argv[2] , argv[3] , argv[4] ) == FAILURE)
            return FAILURE;
    }
    else if ( strcmp ( argv[1] , "-help") == 0 )
    {
        printf("VIEW - please give \"./a.out -v sample.mp3\"  \n");
        printf("EDIT - please give \"./a.out -e sample.mp3 -tag new_name\"  \n");
        printf("FOR EDITING - TAGS--->\n");
        printf("-t  ->  Title name\n");
        printf("-a  ->  Artist name\n");
        printf("-y  ->  Year\n");
        printf("-A  ->  Album\n");
        printf("-c  ->  Genre\n");
        printf("-C  ->  Composer\n");
    }

    return SUCCESS;


}


