#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iconv.h>
#include <errno.h>



int dump_hex(char   *data,  int len)
{
    int  i = 0;
    for (i = 0; i < len; i++)
    {
        if(i!=0 && i%15==0){
            printf("\n");
        }
        
        unsigned  char  c = data[i];
        printf ( "%02X " , c);
    }
    printf  ( "\n" );


    return 0;
}


int iconv_msg(char *inbuf, int inlen, 
                 char *outbuf, int outlen, 
                 const char *from_type, const char *to_type)
{
    iconv_t cd;

    char *ptr_in  = inbuf;
    char *ptr_out = outbuf;

    size_t local_inlen  = inlen;
    size_t local_outlen = outlen;

    cd = iconv_open(to_type, from_type); //gb2312
    if (0 == cd) {
        printf("iconv open failed!\n");
        return -1;
    }
 
    /*iconv返回时， ptr_in,ptr_out已经不再指向原始地址，
     local_inlen,local_outlen返回剩余空间
     local_inlen和local_outlen必须是size_t类型，强制转换也不行，
          否则会导致iconv返回-1，错误errno=84
    */
    if(-1==(int )iconv(cd, (char **)&ptr_in, (size_t *)&local_inlen, 
                           (char **)&ptr_out, (size_t *)&local_outlen))
    {
        /* This is harmless.  Simply move the unused
         * bytes to the beginning of the buffer so that
         * they can be used in the next round.  
         */
        if( errno== EINVAL){
            printf("find some unused bytes, stop");
            //snprintf((char *)outbuf, (unsigned int )outlen, "%s", inbuf);
        }else{
            printf("iconv failed !, errno %d\n",errno);
        }
        
        iconv_close(cd);
        
        return -1; 
    }

    iconv_close(cd);
                           
    return 0;
}


int utf8_to_gbk(char *inbuf, int inlen, char *outbuf, int outlen)
{
    int ret=0;
    
    ret = iconv_msg(inbuf, inlen, outbuf, outlen, "utf8", "gbk");
    if(ret<0){
        printf("utf8 to gbk failed!\n");
        return -1;
    }

    return 0;
}

int gbk_to_utf8(char *inbuf, int inlen, char *outbuf, int outlen)
{
    int ret=0;
    
    ret = iconv_msg(inbuf, inlen, outbuf, outlen, "gbk", "utf8");
    if(ret<0){
        printf("gbk to utf8 failed!\n");
        return -1;
    }

    return 0;
}




int  main( int   argc,  char *argv[])
{
     if  (argc < 3){
         printf( "usage:need two type para\n" );
         
         return  -1;
     }
     
     printf ( "type in %s, type out %s\n" , argv[1], argv[2]);
     
     char  src[100] =  "abcdefg 1234567 中国" ;
     char  des[100] = {0};
     int  srclen = sizeof(src);
     int  deslen = sizeof(des);
     const  char  * srctype = argv[1];
     const  char  * destype = argv[2];

     printf("src:%s\n",src);
     memset(des, 0, sizeof(src));
     dump_hex(des, sizeof(des));
     
     int  ret = utf8_to_gbk(src, srclen, des, deslen);

     printf("\n-------------------------------------\n");
     dump_hex(des, strlen(des));

     
     printf ( "des is : %s\n" , des);
     return  0;
}

