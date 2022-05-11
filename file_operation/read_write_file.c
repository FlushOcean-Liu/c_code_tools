#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>


#define   BUFFER_MAX_SIZE     1024

/*
 * Recurve traverse directiory 
 *
 * @filepath  file name with path
 *
 * return
 *   -1 failed
 *   0  success
 */
int traverse_directory_files(const char *filepath)
{
    int          ret    = 0;
    DIR *        dir    = NULL;
    struct dirent *entry = NULL;
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
    
    if (dir){
        closedir(dir);
        dir = NULL;
    }

    return 0;
}


/*
 * Read each line of the file and print
 *
 * @filepath  file name with path
 *
 * return
 *   -1 failed
 *   0  success
 */
int get_file_line(const char *filepath)
{
    if(!filepath){
        printf("filepath is NULL\n");
        return -1;
    }
    

    FILE       *fp = NULL;
    /*
     * 模式  描述:
     * "r"   以只读方式打开文件，该文件必须存在;
     * "r+"  以读/写方式打开文件，该文件必须存在;
     * "rb+" 以读/写方式打开一个二进制文件;
     * "rt+" 以读/写方式打开一个文本文件，运行读和写;
     * "w"   打开只写文件，若文件存在则长度清零，即该文件
     *       内容消失，若不存在则创建该文件;
     * "w+"  打开可读/写文件，若文件存在则文件长度清零，即
     *       该文件内容会消失，若文件不存在则建立该文件;
     * "a"   以附加的方式打开只写文件，若文件不存在，则会建立
     *       该文件，如果文件存在，写入的数据会被加到文件尾部，
     *       即文件原先内容会被保留（EOF符保留）
     * "a+"  以附加的方式打开可读/写文件，若文件不存在，则会建立
     *       该文件，如果文件存在，写入的数据会被加到文件尾部，
     *       即文件原先内容会被保留（原来EOF符 不保留）
     * "wb"  以只写方式打开或建立一个二进制文件，允许写数据;
     * "wb+" 以读/写方式打开或建立一个二进制文件，允许读写;
     * "wt+" 以读/写方式打开或建立一个文本文件，允许读写;
     * "at+" 以读/写方式打开一个文本文件，允许读或在文件末追加数据;
     * "ab+" 以读/写方式打开一个二进制文件，允许读或在文件末追加数据。
    */
    fp = fopen(filepath, "r");
    if(!fp){
        printf("fopen %s failed !\n",filepath);
        return -1;
    }

    char buff[BUFFER_MAX_SIZE]={0};
    int line=0;
    while(fgets(buff, sizeof(buff), fp)!=NULL){
        line++;
        printf("[line_%04d]%s",line,buff);
    }

    if(fp){
        fclose(fp);
        fp = NULL;
    }

    return 0;
}



/*
 * Copy a file data to new name file
 *
 * @src_file  raw file name with path
 * @dst_file  new file name with path
 *
 * return
 *   -1 failed
 *   0  success
 */
int copy_new_file(const char *src_file, const char *dst_file)
{
    if(!src_file || !dst_file){
        printf("src_file or dst_file is NULL\n");
        return -1;
    }
    FILE  *rfp = NULL;
    FILE  *wfp = NULL;

    size_t ret = 0;

    rfp = fopen(src_file, "r");
    if(!rfp){
        printf("fopen %s failed !\n",src_file);
        return -1;
    }

    wfp = fopen(dst_file, "w");
    if(!wfp){
        printf("fopen %s failed !\n",dst_file);
        return -1;
    }

    char buff[BUFFER_MAX_SIZE]={0};
    int line=0;
    while(fgets(buff, sizeof(buff), rfp)!=NULL){
        line++;
        printf("[line_%04d]%s",line,buff);
        ret = fwrite(buff, strlen(buff), 1, wfp);
        if(ret!=1){
            printf("write failed!\n");
            return -1;
        }
    }

    if(rfp){
        fclose(rfp);
        rfp = NULL;
    }

    if(wfp){
        fclose(wfp);
        wfp = NULL;
    }
    
    return 0;
}


void help_usage(void)
{
    printf("help:\n");
    printf("example [rwt] [filepath]\n");
}


int main(int argc, char *argv[])
{
    //traverse_directory_files("/home/liugh");

    //get_file_line("./CMakeCache.txt");

    copy_new_file("./CMakeCache.txt", "./new_file.txt");

    return 0;
}
