#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>


#define   BUFFER_MAX_SIZE     1024

//遍历文件夹及其子目录
int traverse_directory_files(const char *filepath)
{
    int          ret    = 0;
    DIR *        dir    = NULL;
    struct dirent *entry = NULL;;
    char sub_filepath[BUFFER_MAX_SIZE] = { 0 };

    if (!(dir = opendir(filepath))){
        printf("only single file: %s\n", filepath);

        return -1;
    }

    while ((entry = readdir(dir)) != NULL){
    
        if(strncmp(entry->d_name,".",1)==0 || 
          strncmp(entry->d_name,"..",2)==0){
            continue;
        }
        
        snprintf(sub_filepath, sizeof(sub_filepath), 
                        "%s/%s", filepath, entry->d_name);
        if (entry->d_type == DT_DIR){
            ret = traverse_directory_files(sub_filepath);
            if (ret < 0)
                return -1;
        }else{
            printf("find file:%s\n",sub_filepath);
        }
    }
    
    if (dir)
        closedir(dir);

    return 0;
}



int read_file(char *filepath)
{
    FILE       *fp;
    fp = fopen(filepath, "w");
    if(!fp){
        printf("fopen %s failed !\n",filepath);
        return -1;
    }

    char buff[BUFFER_MAX_SIZE]={0};
    int line=0;
    while(fgets(buff, sizeof(buff), fp)!=NULL){
        line++;
        printf("[line_%04d]%s\n",line,buff);
    }

    return 0;
}

int write_file(const char *src_file, const char *dst_file)
{



    return 0;
}

int main(int argc, char *argv[])
{


    return 0;
}
