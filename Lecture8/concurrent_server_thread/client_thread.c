#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 8080

typedef struct {
    int fd;
    char *msg;
} thread_params;

void *thread_handle(void *vargp) {
    char buf[1024] = {0};
    printf("entering thread %lu\n", pthread_self());
    thread_params params = *(thread_params *) vargp;
    int fd = params.fd;
    char* msg = params.msg;

    write (fd, msg, strlen(msg));
    printf("Sent from socket %d: %s\n", fd, msg);
    read(fd, buf, 1024); //receive msg from server
    printf("Received from sock %d: %s\n", fd, buf);
    return 0;
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock1, sock2, valread;
    struct sockaddr_in serv_addr;
    char *hello1 = "Hello from client 1";
    char *hello2 = "Hello from client 2";
    thread_params params1, params2;
    params1.msg = hello1;
    params2.msg = hello2;
    pthread_t tid[2]; 

    if ((sock1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return -1;
    }
    if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return -1;
    }
  
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IP addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    // connect to server 
    if (connect(sock1, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        return -1;
    }
    params1.fd = sock1;
    if (pthread_create(&tid[0], NULL, thread_handle, &params1) < 0) {
        perror("pthread_create");
        return -1;
    }
    // connect using sock2
    if (connect(sock2, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        return -1;
    } 
    params2.fd = sock2;
    if (pthread_create(&tid[1], NULL, thread_handle, &params2) < 0) {
        perror("pthread_create");
        return -1;
    }
    // wait for threads
    printf("Main thread wants for handle threads ...\n");
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    return 0;
}
