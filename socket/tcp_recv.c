#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#define DATA_BUFFER_SIZE  1024

struct socket_args{
    int conn_fd;
};



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


int create_connect(void)
{
    pthread_t tid;
    struct sockaddr_in  client_addr,server_addr;
    
    int  listen_fd=socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family= AF_INET;

    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_addr.sin_port=htons(9999);

    bind(listen_fd, (struct sockaddr*)&server_addr,sizeof(server_addr));
    listen(listen_fd, 10000);
    socklen_t cli_len;
    int conn_fd=0;
    int ret=0;
    while(1){
        conn_fd=accept(listen_fd, (struct sockaddr*)&client_addr, &cli_len);
        if(conn_fd<0){
            printf("accept conn_fd < 0 ,=%d\n",conn_fd);
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
        //printf("[Client] get connection ip:%s,port:%d\n",
        //                      inet_ntoa(client_addr.sin_addr),
        //                      ntohs(client_addr.sin_port));
        ret=pthread_create(&tid, NULL,socket_receive_thread, (void*)child_args);
        if(0!=ret){
            printf("create thread failed!\n");
            exit(-1);
        }
    }

}

int main()
{

    create_connect();

    return 0;
}
