#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>



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
    printf("thread name:%s\n",thread_name);
    
    int count=0;
    while(1){
        count++;
        printf("thread func %d\n",count);
        sleep(1);
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
    pthread_t pthread;

    int status = 0;

    /* 此次传递给线程的参数要独立分配空间,
    在一次创建多个线程时容易只给一个地址空间的参数*/
    char thread_name[20]={0};
    strncpy(thread_name, "thread_test",strlen("thread_test"));
    status = pthread_create(&pthread, NULL, thread_func, (void *)thread_name);
    if(status != 0) {
        printf( "error on create thread\n");
        exit(-1);
    }

    /* 主线程等待子线程的终止 */
    pthread_join(pthread, NULL);

    return 0;
}
