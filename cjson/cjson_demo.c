#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"


int  main()
{
//    char *s = "{\"list\":{\"name\":\"xiao hong\",\"age\":10},\"other\":{\"name\":\"hua hua\"}}";
    
    FILE *fp=fopen("test.json","rb");
    fseek(fp, 0, SEEK_END);
    int len=ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buff=(char *)malloc(sizeof(char)*len);
    fread(buff, 1, len, fp);
    fclose(fp);

    printf("%s\n",buff);


    cJSON *root = cJSON_Parse(buff);
    if(!root) {
        printf("get root failed !\n");
        return -1;
    }

    printf("===================== read list value =====================\n");
    cJSON *js_list = cJSON_GetObjectItem(root, "list");
    if(!js_list) {
        printf("no list!\n");
        return -1;
    }
    printf("list_type:%d\n",js_list->type);

    cJSON *name = cJSON_GetObjectItem(js_list, "name");
    if(!name) {
        printf("No name !\n");
        return -1;
    }
    printf("\tname_type:%d",name->type);
    printf("\tname: %s\n",name->valuestring);

    cJSON *age = cJSON_GetObjectItem(js_list, "age");
    if(!age) {
        printf("no age!\n");
        return -1;
    }
    printf("\tage_type:%d", age->type);
    printf("\tage:%d\n",age->valueint);




    printf("===================== read other value =====================\n");
    cJSON *js_other = cJSON_GetObjectItem(root, "other");
    if(!js_other) {
        printf("no list!\n");
        return -1;
    }
    printf("list_type:%d\n",js_other->type);

    cJSON *js_name = cJSON_GetObjectItem(js_other, "name");
    if(!js_name) {
        printf("No name !\n");
        return -1;
    }
    printf("\tname_type is %d",js_name->type);
    printf("\tname:%s\n",js_name->valuestring);

    printf("##################### 以数据组方法遍历列表 #################\n");
    cJSON *js_field = cJSON_GetObjectItem(root, "field");
    if(!js_field) {
        printf("field no list!\n");
        return -1;
    }

    int size=cJSON_GetArraySize(js_field);
    int i=0,j=0;
    cJSON *item;
    for(i=0;i<size;i++){
        item = cJSON_GetArrayItem(js_field, i);
        if(NULL==item){
            continue;
        }
        printf("%s:\t",item->string);
        cJSON *citem=cJSON_GetObjectItem(js_field, item->string);
        if(NULL==citem){
            continue;
        }
        
        int num=cJSON_GetArraySize(citem);
        for(j=0;j<num;j++){
           cJSON *array=cJSON_GetArrayItem(citem, j);
           printf("%s\t",array->valuestring);
        }
        printf("\n");
    }

    if(root){
        cJSON_Delete(root);
    }

    return 0;
}
