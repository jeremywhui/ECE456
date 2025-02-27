// TCP server setup using addrinfo
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024
int main(int argc, char const *argv[])
{
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    const char *port = "http";    // default service/port, can be modified by optional argument
    int listenfd, connfd, size;
    char buf[MAXLINE];
    char *servmsg = "ACK from server";

    if (argc > 1) {
        port = argv[1];
    }

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    if ((status = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }
   
    // create socket 
    if ((listenfd = socket(servinfo->ai_family, servinfo->ai_socktype, 0)) < 0) 
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    // bind to address
    if (bind(listenfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo); // free the linked-list
    if (listen(listenfd, 1024) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    // accept client
    if ((connfd = accept(listenfd, NULL, NULL)) < 0) 
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    // read from client
    if ( (size = read(connfd, buf, MAXLINE-1)) < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buf[size] = '\0';
    printf("Received: %s\n", buf);
    // send message to client
    printf("Send server message to client\n");
    write(connfd, servmsg, strlen(servmsg));
    close(connfd);
    return 0;
}
