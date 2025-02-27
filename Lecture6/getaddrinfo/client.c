// client using getaddrinfo() to connect 
//
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
    int sock, size;
    char buf[MAXLINE];
    char *clientmsg = "request from client";

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <domain name or dotted-decimal> [port]\n", argv[0]);
        exit(1);
    }
    if (argc > 2) {
        port = argv[2];
    }

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

    if ((status = getaddrinfo(argv[1], port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }
    // create socket 
    if ((sock = socket(servinfo->ai_family, servinfo->ai_socktype, 0)) < 0) 
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    // connect to server socket 
    if (connect(sock, servinfo->ai_addr, servinfo->ai_addrlen) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(servinfo); // free the linked-list

    // send client request 
    printf("Send client message to server\n");
    write(sock, clientmsg, strlen(clientmsg));

    // read server message
    if ( (size = read(sock, buf, MAXLINE-1)) < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buf[size] = '\0';
    printf("receid: %s\n", buf);

    close(sock);
    return 0;
}
