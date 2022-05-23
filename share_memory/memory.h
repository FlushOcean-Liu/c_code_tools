#ifndef  __MEMORY_H__
#define  __MEMORY_H__


#define SHARE_ID  123456
#define NAME_SIZE 32


struct shm_data{
    char name[NAME_SIZE];
    int  id;
};


#endif 
