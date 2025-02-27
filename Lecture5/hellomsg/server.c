// Server side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address, cliaddr;
    int opt = 1;
    int addrlen = sizeof(cliaddr);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    char ip_str[INET_ADDRSTRLEN];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl( INADDR_ANY );
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
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
    // accept connection from client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&cliaddr, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer, 1024);
    if (inet_ntop(AF_INET, &cliaddr.sin_addr, ip_str, sizeof(ip_str)) 
		    	== NULL) 
    {
	    perror("inet_ntop error");
	    exit(EXIT_FAILURE);
    } 
    printf("%s (%s:%d)\n", buffer, ip_str, ntohs(cliaddr.sin_port));
    write(new_socket , hello , strlen(hello));
    printf("Hello message sent to (%s:%d)\n", ip_str, 
		    ntohs(cliaddr.sin_port));
    return 0;
}
