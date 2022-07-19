#include <stdio.h>
#include <stdlib.h>

#define  ARRAY_MAX_NUM   10

int split(char *str, char *ptr[],  char separator, int max_index)
{
    char *start_c = str;
    char *end_c   = str;

    int index = 0;

    while (*end_c!='\0'){
        if (*end_c == separator){
            if (start_c == end_c){
                ptr[index] = NULL;
            }else{
                ptr[index] = start_c;
            }
            *end_c = '\0';
            index++;
            if(index>=max_index){
                break;
            }
            start_c = end_c + 1;
        }
        end_c++;
    }
 
    if (start_c != end_c){
        ptr[index] = start_c;
    }else{
        index--;
    }
 
    return index;
}

int main()
{
    int i = 0;
    char str[] = "11:22:33:44:55:66";
    char *ptr[ARRAY_MAX_NUM];
 
	printf("[raw string] %s\n",str);
    int len = split(str, ptr, ':', ARRAY_MAX_NUM);
    for (i = 0; i <= len; i++){
        printf("[%d]%s\n", i,ptr[i]);
    }
 
    return 0;
}
