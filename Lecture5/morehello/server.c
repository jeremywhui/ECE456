// Server side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#define PORT 9000
// TCP server read msg from client and send server hello message back
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    char buffer[1024] = {0};
    char *hello = "Hello from C server";
    int port = PORT;

    if (argc > 1) {
        port = atoi(argv[1]);
    }
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl( INADDR_ANY );
    address.sin_port = htons( port );

    // bind socket to the address/port 
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 1024) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1) {
        // accept connection from client
        if ((new_socket = accept(server_fd, NULL, NULL)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer);
        write(new_socket , hello , strlen(hello));
        close( new_socket );
    }
    return 0;
}
