#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

int get_local_ip(char *ip_list, int len)
{
    int fd;
    int number = 0;
        int ret = 0;
        int i;
        char *tmp = NULL;
        int offset=0;

    struct ifreq buf[INET_ADDRSTRLEN];
    struct ifconf conf_data;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd<0){
        return -1;
    }


    conf_data.ifc_len = sizeof(buf);
    // caddr_t,linux内核源码里定义的：typedef void *caddr_t；
    conf_data.ifc_buf = (caddr_t)buf;
    if (ioctl(fd, SIOCGIFCONF, (char *)&conf_data)){
        return -1;
    }

    number = conf_data.ifc_len/sizeof(struct ifreq);
    while (number-- > 0){
                if (ioctl(fd, SIOCGIFADDR, (char *)&buf[number])){
                        break;
                }


        tmp=(inet_ntoa(((struct sockaddr_in*)(&buf[number].ifr_addr))->sin_addr));
                if(NULL==tmp || len<(strlen(tmp)+offset)){
                        break;
                }
                strcat(ip_list, tmp);
                offset+=strlen(tmp);
                ip_list[offset]=',';
                offset+=1;
    }
        if(offset>0)
        ip_list[offset-1]='\0';

    close(fd);

    return 0;
}

int main()

{
    char ip_list[512]={0};
    memset(ip_list, 0, sizeof(ip_list));
    get_local_ip(ip_list, sizeof(ip_list));

    printf("get ip list[%s]\n",ip_list);
    return 0;

}
