#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE_BUF     1024

/*
 * Read each line of the file and print
 *
 * @filename  file name with path
 *
 * return
 *   -1 failed
 *   0  success
 */
int get_file_line(char *filename)
{
    if(!filename){
        return -1;
    }
    FILE *fp = NULL;

    fp = fopen(filename, "r");
    if(!fp){
        printf("can not open file %s\n",filename);
        return -1;
    }

    char line[MAX_LINE_BUF]={0};
    int index=0;
    while(!feof(fp)){
        index++;
        if(fgets(line, MAX_LINE_BUF, fp)){
            line[strlen(line)-1]='\0';
            printf("line%04d:%s\n",index,line);
        }
    }
    
    return 0;
}

