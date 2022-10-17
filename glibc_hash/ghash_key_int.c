#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>

struct node_data{
  char *key;
  int   value;  
};

struct node_data array_field[5]=
{
    {"key1", 1},
    {"key2", 2},
    {"key3", 3},
    {"key4", 4},
    {"key5", 5},
};


#define INIT_ELEMENT_NUM   32
static GHashTable *g_hash = NULL;

void print_hash_element(gpointer key, gpointer value, gpointer user_data) 
{
    printf("[%s]key:%s value:%s\n",(char *)user_data, (char *)key, (char *)value);
}

void destroy_key(gpointer hash_key) 
{
    if(hash_key){
        printf("destroy key:%s\n",(char *)hash_key);
        hash_key = NULL;
    }
}

void destroy_value(gpointer hash_value) 
{
    if(hash_value){
        printf("destroy value:%d\n",(int *)hash_value);
        hash_value = NULL;
    }
}


int init_hash_table(void)
{
    int ret = 0;
    g_hash = g_hash_table_new_full(g_str_hash, g_int_equal,
                                 destroy_key, destroy_value);
    if(!g_hash){
        printf("g_hash create failed!\n");
        return -1;
    }

    int i = 0;
    char key[64] ;
    char value[64] ;
    for(i = 0; i<sizeof(array_field)/sizeof(array_field[0]);i++){
        g_hash_table_insert(g_hash, (gpointer)array_field[i].key, (gpointer)array_field[i].value);
    }

    return 0;
}




int main()
{

    if(init_hash_table()<0){
        printf("hash table init failed!\n");
        exit(-1);
    }

    printf("------------------lookup key------------------------\n");
    int i;
    gpointer result;
	char buff[64];
    for(i = 0; i<sizeof(array_field)/sizeof(array_field[0]);i++){
	    snprintf(buff, 64, "%s",array_field[i].key);
        result=g_hash_table_lookup(g_hash, (gpointer)buff);
        if(result)
            printf("find key:%s result:%d\n",buff,(int *)result);
    
    }

    printf("\n------------------free hashtable------------------------\n");
    if(g_hash)
        g_hash_table_destroy(g_hash);
    g_hash=NULL;

    return 0;
}
