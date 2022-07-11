#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFF_SIZE 1024

int main()
{
    FILE *fp;
    char buff[MAX_BUFF_SIZE]={0};

    if((fp=popen("ls ./", "r"))==NULL){
        perror("Fail to popen\n");
        exit(1);
    }

    while(fgets(buff, MAX_BUFF_SIZE, fp)!=NULL){
        printf("%s", buff);
    }

    if(fp)
        pclose(fp);

    return 0;

}
