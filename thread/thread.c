#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#define THREAD_NUM  5
#define MAX_NAME_SIZE 64

void *thread_func(void *argv)
{

    /*
    int core_affinity = 0x01;
    if (core_affinity >= 0) {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core_affinity, &cpuset);

        if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0)
            printf("Error while binding thread to core %d\n",core_affinity);
    }
    */
    char *thread_name=(char *)argv;
    if(!thread_name){
        printf("thread name is NULL\n");
        return NULL;
    }
    printf("thread name:%s\n",thread_name);
    
    int count=0;
    while(1){
        count++;
        printf("%s times %d\n",thread_name, count);
        sleep(1);
    }

    if(thread_name){
        free(thread_name);
    }

    return NULL;
}

void help_usage(void)
{
    printf("help:\n");
    printf("example [rwt] [filepath]\n");
}


int main(int argc, char *argv[])
{
    pthread_t pthread[THREAD_NUM];
    int i;

    int status = 0;

    /* 此次传递给线程的参数要独立分配空间,
    在一次创建多个线程时容易只给一个地址空间的参数*/
    char *thread_name=NULL;
    for(i=0;i<THREAD_NUM;i++){
        thread_name=malloc(sizeof(char)*MAX_NAME_SIZE);
        if(!thread_name){
            printf("thread name malloc failed\n");
            exit(-1);
        }
        snprintf(thread_name,MAX_NAME_SIZE,"thread_%02d",i);
        status = pthread_create(&pthread[i], NULL, thread_func, (void *)thread_name);
        if(status != 0) {
            printf( "error on create thread\n");
            exit(-1);
        }
    }
    
    /* 主线程等待子线程的终止 */
    for(i=0;i<THREAD_NUM;i++){
        pthread_join(pthread[i], NULL);
    }

    return 0;
}
