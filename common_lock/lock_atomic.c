#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <linux/types.h>



/* 定义一个原子变量 */
static atomic_t g_atomic = ATOMIC_INIT(1);

/* 共享资源 */
static volatile int g_value = 0;
 

void *thread_func_01(void *argv)
{
    while(1){
        atomic_add(1,&g_atomic);
        printf("in thread %lu value=%d\n", pthread_self(), g_atomic);
        sleep(1);
    }
}


void *thread_func_02(void *argv)
{
    while(1){
        atomic_add(2, &g_atomic);
        printf("in thread %lu value=%d\n", pthread_self(), g_atomic);

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

