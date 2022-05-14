#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main() {
    int socket_desc;
    struct sockaddr_in server;
    char *message;
    
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == socket_desc) {
        perror("cannot create socket");
        exit(1);
    }
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(9999);
    
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect error");
        return 1;
    }
    
    printf("connect success\n");
    message = "hello world";
    if (send(socket_desc, message, strlen(message), 0) < 0) {
        printf("send error");
        return 2;
    }
    
    printf("message send success");
    return 0;
}

