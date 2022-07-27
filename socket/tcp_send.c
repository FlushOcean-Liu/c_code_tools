#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h> 
#include <unistd.h>


#define  MAX_BUFF_SIZE   1024

struct sockaddr_in g_server;
int                g_socket_fd;

static int client_socket_init(void)
{
    g_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    g_server.sin_family = AF_INET;
    g_server.sin_port = htons(9999);

    g_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == g_socket_fd) {
        perror("cannot create socket");
        exit(1);
    }

    if (connect(g_socket_fd, (struct sockaddr *)&g_server, sizeof(g_server)) < 0) {
        perror("connect error");
        return -1;
    }

    return 0;
}

static int client_socket_reconnect(void)
{
    g_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == g_socket_fd) {
        perror("cannot create socket");
        return -1;
    }
    if (connect(g_socket_fd, (struct sockaddr *)&g_server, sizeof(g_server)) < 0) {
        perror("connect error");
        return -1;
    }
    
    return 0;
}


static int client_socket_send(char *data, int data_len)
{
    if(!data || data_len==0){
        return -1;
    }
    
    int ret=0;
    struct tcp_info m_info;
    int infoLen = sizeof(m_info);
    getsockopt(g_socket_fd, IPPROTO_TCP, TCP_INFO, &m_info,
                      (socklen_t*)&infoLen);
    if(m_info.tcpi_state==TCP_ESTABLISHED){
        int send_len=0;
        while(send_len!=data_len){
            int left_len = data_len-send_len;
            /*
             * ssize_t send(int sockfd, const void *buf, size_t len, int flags);
             * flags:
             *       0-此时send为阻塞式发送，即发送不成功会一直阻塞；
             *       MSG_NOSIGNAL-标识当连接被关闭时不会产生SIGPIPE信号；
             *       MSG_DONTWAIT，表示非阻塞发送；
             *
            */
            ret=send(g_socket_fd, data+send_len, left_len, MSG_NOSIGNAL);
            if(ret<0){
                break;
            }
            send_len+=ret;
        }
    }else{
        printf("socket Reconnecting....\n");
        close(g_socket_fd);
        sleep(1);
        if(0==client_socket_reconnect())
            printf("Socket Reconnnecting success!\n");

    }

    return 0;
}


int main() 
{
    if(client_socket_init()<0){
        printf("client socket init failed\n");
    }
   
    char buff[MAX_BUFF_SIZE]={0};

    while(1){
        printf("input:\n");
        fgets(buff,MAX_BUFF_SIZE-1,stdin);
        client_socket_send(buff, strlen(buff));
    }
    
    
    printf("message send success");
    return 0;
}

