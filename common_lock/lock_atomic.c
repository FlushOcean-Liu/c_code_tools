#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <linux/types.h>


/*
 *
 * 在Linux2.6.18之后，系统便删除了<asm/atomic.h>和<asm/bitops.h>，<alsa/iatomic.h>
 * 在Linux操作系统下GCC提供了内置的原子操作函数__sync_*，更方便程序员调用
 *
 * gcc从4.1.2提供了__sync_*系列的built-in函数，用于提供加减和逻辑运算的原子操作。可以对1,2,
 * 4或8字节长度的数值类型或指针进行原子操作，其声明如下:
 * type __sync_fetch_and_add (type *ptr, type value, ...)
 * type __sync_fetch_and_sub (type *ptr, type value, ...)
 * type __sync_fetch_and_or (type *ptr, type value, ...)
 * type __sync_fetch_and_and (type *ptr, type value, ...)
 * type __sync_fetch_and_xor (type *ptr, type value, ...)
 * type __sync_fetch_and_nand (type *ptr, type value, ...)
 * type __sync_add_and_fetch (type *ptr, type value, ...)
 * type __sync_sub_and_fetch (type *ptr, type value, ...)
 * type __sync_or_and_fetch (type *ptr, type value, ...)
 * type __sync_and_and_fetch (type *ptr, type value, ...)
 * type __sync_xor_and_fetch (type *ptr, type value, ...)
 * type __sync_nand_and_fetch (type *ptr, type value, ...)
 *
*/

static volatile int g_value = 0;
 

void *thread_func_01(void *argv)
{
    while(1){
        __sync_fetch_and_add(&g_value, 1);
        printf("in thread_01 %lu value=%d\n", pthread_self(), g_value);
        sleep(1);
    }
}


void *thread_func_02(void *argv)
{
    while(1){
        __sync_fetch_and_add(&g_value, 2);
        printf("in thread_02 %lu value=%d\n", pthread_self(), g_value);

        sleep(1);
    }
}


int main()
{
    pthread_t th1, th2;

    int status = 0;
    status = pthread_create(&th1, NULL, thread_func_01, NULL);
    if(status != 0) {
        printf( "error on create thread_func_01\n");
        exit(-1);
    }

    status = pthread_create(&th2, NULL, thread_func_02, NULL);
    if(status != 0) {
        printf( "error on create thread_func_02\n");
        exit(-1);
    }

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    return 0;
}

