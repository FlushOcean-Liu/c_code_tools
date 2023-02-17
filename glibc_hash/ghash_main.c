#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>


#define INIT_ELEMENT_NUM   32
static GHashTable *g_hash = NULL;

void print_hash_element(gpointer key, gpointer value, gpointer user_data) 
{
    printf("[%s]key:%s value:%s\n",(char *)user_data, (char *)key, (char *)value);
}

void destroy_key(gpointer hash_key) 
{
    if(hash_key){
        printf("delete key:%s\n",(char *)hash_key);
        free(hash_key);
        hash_key = NULL;
    }
}

void destroy_value(gpointer hash_value) 
{
    if(hash_value){
        printf("delete value:%s\n",hash_value);
        free(hash_value);
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
    for(i = 0; i< INIT_ELEMENT_NUM;i++){
        snprintf(key,sizeof(key),"%d",i);
        snprintf(value,sizeof(value), "%d", random());
        
        char* pkey   = strdup(key);
        char* pvalue = strdup(value);

        if( !pkey || !pvalue){
            printf("key or value malloc failed\n");
            return -2;
        }

        g_hash_table_insert(g_hash, pkey,pvalue);
    }

    return 0;
}




int main()
{
    char buff[32] = {0};

    if(init_hash_table()<0){
        printf("hash table init failed!\n");
        exit(-1);
    }

    printf("------------------lookup key------------------------\n");
    char search_key[64]={0};
    int s_digit=16;
    snprintf(search_key,64,"%d",s_digit);
    gpointer result=NULL;
    result=g_hash_table_lookup(g_hash, (gconstpointer)search_key);
    if(result)
        printf("find key:%s result:%s\n",search_key,(char *)result);


    printf("\n------------------hash elements------------------------\n");
    memcpy(buff,"element",12);
    if(NULL != g_hash) {
        g_hash_table_foreach(g_hash, print_hash_element, buff);
    }

    printf("\n------------------ clean hashtable------------------------\n");
    if(g_hash) /* 清空hash表中数据*/
        g_hash_table_remove_all(g_hash);
    

    printf("\n------------------destroy hashtable------------------------\n");
    if(g_hash) /* 销毁hash表，销毁后不能再用*/
        g_hash_table_destroy(g_hash);
    g_hash=NULL;

    return 0;
}
