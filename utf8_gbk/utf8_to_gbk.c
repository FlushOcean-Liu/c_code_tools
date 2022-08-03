#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>

typedef long unsigned int    size_t;

int utf8_to_gbk(char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
    iconv_t cd;    
    char **pin  = &inbuf;
    char **pout = &outbuf;
    
    cd = iconv_open("gbk", "utf-8"); //gb2312
    if (0 == cd) 
        return -1;
    if (-1 == iconv(cd, pin, &inlen, pout, &outlen))
    {
        iconv_close(cd);
        return -1;
    }
 
    iconv_close(cd);
    return 0;
}

int main(int argc, char *argv[])
{

	char   outbuf[1025]={0};
    size_t outlen=1024;

    utf8_to_gbk(argv[1], strlen(argv[1]), outbuf, outlen);

	printf("outbuf:%s\n",outbuf);

	return 0;
}

