#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

void get_file_creation_time(char *path) {
    struct stat attr;
    stat(path, &attr);
    //printf("Last modified time: %s\n", ctime(&attr.st_mtime));
    printf("Last modified time: %d\n", attr.st_mtime);
}


int  main(int argc, char *argv[])
{
        if(argc!=2){
                printf("input check file\n");
                printf("./a.out test.txt\n");
                return -1;
        }
        char *dir=argv[1];

    get_file_creation_time(dir);

}
