#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/*
 * assert的作用是计算表达式，如果其值未假（即为0），那么它先向stderr打印一条出错信息，
 * 然后调用abort来终止程序运行。
 *
 *
 * */


int main(void)
{
    FILE *fp;

    fp = fopen("test.txt", "w");
    assert(fp);

    printf("fopen test.txt\n");
    fclose(fp);
    
    fp = fopen("noexit.txt", "r");
    assert(fp);

    printf("fopen noexit.txt\n");
    fclose(fp);


    return 0;
}
