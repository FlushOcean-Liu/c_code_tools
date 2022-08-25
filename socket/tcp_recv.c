#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <netinet/tcp.h> 


#define DATA_BUFFER_SIZE  1024

#define MAX_CLIENT        10000

struct socket_args{
    int conn_fd;
};

#define MAX(x,y) ((x)>(y))?(x):(y)


/*收包线程处理函数*/
void *socket_receive_thread(void *args)
{
    struct socket_args *child_args=(struct socket_args *)args;
    int conn_id=child_args->conn_fd;

    printf("child conn_id:%d\n",conn_id);
    char buff[DATA_BUFFER_SIZE]={0};
    int  size =0;
    while (1) {
        size = recv(conn_id, buff, 512,MSG_WAITALL);
        if(size>0){
            printf("RECV:%s\n",buff);
        }
    }

    
    close(conn_id);
    free(child_args);
    pthread_detach(pthread_self());

    return NULL;
}


int create_socket_thread(void)
{
    pthread_t tid;
    struct sockaddr_in  client_addr,server_addr;
    int opt;
    int optlen;
    
    
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family= AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_addr.sin_port=htons(9999);

    int  sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == 0) {
        perror("init server socket error\n");
        return -1;
    }



    /* 想要达成功能：绑定端口前，清除之前的绑定
	 *
	 * 实际虚拟机中，导致accept一直返回-1，注释掉即正常，具体原因需要深入探究
	 * */
    opt=1;
    optlen=sizeof(opt);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, optlen);    
    /* 经过测试验证导致accept一直返回-1，是因为accept的第三个参数没有赋值，不是setsockopt原因 */
	
	bind(sockfd, (struct sockaddr*)&server_addr,sizeof(server_addr));
    listen(sockfd, MAX_CLIENT);
    socklen_t cli_len;
    int       conn_fd = 0;
    int       ret     = 0;
    
    while(1){
	    cli_len=sizeof(struct sockaddr); // 此次cli_len不赋值，会导致accept返回-1
        conn_fd=accept(sockfd, (struct sockaddr*)&client_addr, &cli_len);
        if(conn_fd<0){
            printf("accept conn_fd < 0 ,conn_fid=%d\n",conn_fd);
            usleep(1);
            continue;
        }
        
        struct socket_args *child_args=(struct socket_args *)malloc(sizeof(struct socket_args *));
        child_args->conn_fd=conn_fd;
        if(child_args==NULL){
            printf("child_args is NULL\n");
            continue;
        }
        child_args->conn_fd=conn_fd;
        ret=pthread_create(&tid, NULL,socket_receive_thread, (void*)child_args);
        if(0!=ret){
            printf("create thread failed!\n");
            exit(-1);
        }
    }

}

int create_socket_select(void)
{
    struct sockaddr_in  client_addr,server_addr;
    int opt;
    int optlen;
    
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family= AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_addr.sin_port=htons(9999);

    int  sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == 0) {
        perror("init server socket error\n");
        return -1;
    }

    /*绑定端口前，清除之前的绑定*/
    opt=1;
    optlen=sizeof(opt);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, optlen);
    
    bind(sockfd, (struct sockaddr*)&server_addr,sizeof(server_addr));

    listen(sockfd, MAX_CLIENT);
    socklen_t cli_len;
    int       conn_fd = 0;
    int       ret     = 0;
    int       i;
    
    int user_link[MAX_CLIENT];
    int userfd[MAX_CLIENT];
    char buffer[DATA_BUFFER_SIZE];

    fd_set sockset;
    int maxfd = 0;
    struct timeval tv;
    
    listen(sockfd, MAX_CLIENT);
    for (i = 0; i < MAX_CLIENT; i++) {
        user_link[i] = 0;
    }

    FD_ZERO(&sockset);
    FD_SET(sockfd, &sockset);

    maxfd = MAX(maxfd, sockfd + 1);
    int userindex = 0;
    while(1) {
        
        select(maxfd, &sockset, NULL, NULL, &tv);
        if (FD_ISSET(sockfd, &sockset)) {
            conn_fd = accept(sockfd, (struct sockaddr *) &client_addr, &cli_len);
            if (conn_fd <= 0)
                perror("accept");
            else {
                user_link[userindex] = 1;
                userfd[userindex] = conn_fd;
                FD_SET(conn_fd, &sockset);
                maxfd = MAX(maxfd, conn_fd + 1);
            }
        }

        int ifd = 0;
        for (i = 0; i < MAX_CLIENT; i++) {
            if (user_link[i] == 1 && (FD_ISSET(userfd[i], &sockset))) {
                memset(buffer, 0 ,DATA_BUFFER_SIZE);
                unsigned int length = recv(userfd[i], buffer, DATA_BUFFER_SIZE, 0);
                ifd++;
                if (length <= 0) {
                    close(userfd[i]);
                    user_link[i] = 0;
                    FD_CLR(userfd[i], &sockset);
                } else {
                    printf("RECV:%s\n",buffer);
                }
            }
        }

        FD_ZERO(&sockset);
        FD_SET(sockfd, &sockset);
        int j;
        for (j = 0; j < MAX_CLIENT; j++) {
            if (user_link[j] == 1)
                FD_SET(userfd[j], &sockset);
        }
    }

    return 0;
 }


int main()
{

    //create_socket_thread();

    create_socket_select();

    return 0;
}
