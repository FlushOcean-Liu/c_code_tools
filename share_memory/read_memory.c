#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/shm.h>

#include "memory.h"

int read(void)
{
    /* 获取shm_id, 在不同进程中这是唯一的，获取和KEY有关 */
    int shm_id = shmget(SHARE_ID, sizeof(struct shm_data), 0666 | IPC_CREAT);
    if(shm_id==-1){
        printf(" shmget share memory failed !\n");
        return 0;
    }

    /* 创建一个指针，用来指向共享内存 */
    void *addr_to_shm = NULL;

    struct shm_data *data = NULL;
    addr_to_shm = shmat(shm_id, (void *)0, 0);
    if(!addr_to_shm){
        printf("shmat failed\n");
        return 0;
    }
    printf("\n Memory attached at %X\n",*(int *)addr_to_shm);

    data = (struct shm_data *)addr_to_shm;
    while(1){
        if(0==strncmp(data->name,"end",strlen(data->name))){
            break;
        }
        printf("name:%s, id:%d\n",data->name,data->id);
        sleep(1);
    }

    shmdt(addr_to_shm);

    shmctl(shm_id, IPC_RMID, 0);

    return 0;
}


int main()
{
    
    read();

    return 0;
}

