#include <stdio.h>    
#include <string.h>    
#include <unistd.h>    
    
void process_bar(int number)    
{    

    int i = 0;    
    char proc[102];  
    memset(proc, '>', sizeof(proc));    
    
    int index=0;
    for(i=1;i<=number;i++)    
    {  
        //C语言格式控制时默认右对齐，所以要在前面加-变成左对齐    
        fflush(stdout);//刷新屏幕打印  
        index=(i*100)/number;
        proc[index] = '\0';  
        usleep(100);//以微秒为单位的sleep  
        printf("[%-100s] [%0.04f%%][%d]\r", proc, (double)(i*100.0)/number,i);                                                                                                                                  
        memset(proc, '>', sizeof(proc));    
    }  
    printf("\n");  
}                                                                                                                                                   
                                                                                                                                             
int main(int argc, char *argv[])                                                                                                                                   
{          
    if(argc!=2){
        printf("usage:\n");
	printf("./a.out [number]\n");
    }
    int number=(int)atoi(argv[1]);                                                                                                                                  
    
    process_bar(number);                                                                                                                              
    
    return 0;                                                                                                                              
}
