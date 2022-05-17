#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>


struct data_st{
    int id;
    pthread_mutex_t mutex;
};

struct data_st g_data;

void *thread_func_01(void *argv)
{
    while(1){
        pthread_mutex_lock(&g_data.mutex);
        g_data.id++;
        printf("thread1 print id=%d\n",g_data.id);
        pthread_mutex_unlock(&g_data.mutex);

        sleep(1);
    }
}


void *thread_func_02(void *argv)
{
    while(1){
        pthread_mutex_lock(&g_data.mutex);
        g_data.id++;
        printf("thread2 print id=%d\n", g_data.id);
        pthread_mutex_unlock(&g_data.mutex);

        sleep(1);
    }
}


int main()
{
    pthread_t th1, th2;
    
    pthread_mutex_init(&g_data.mutex, NULL);

    int status=0;
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

