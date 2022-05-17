#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>



/*
    自旋锁：连续循环等待，专为防止多处理器并发引入的锁，适用于快速完成的代码
    特点：自旋不能抢占，可以中断；


*/


int main()
{
    printf("hello\n");

    return 0;
}
