// Client side – v6hello client: send hello to Ipv6 server and read back
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
  
int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in6 serv_addr;
    char *hello = "Hello from IPv6 client";
    char buffer[1024] = {0};
    const char *server_ip = "::1";  // IPv6 loopback address
    if (argc > 1) {
	    server_ip = argv[1];
    }

    if ((sock = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(PORT);

    // Convert IP addresses from text to binary form
    if(inet_pton(AF_INET6, server_ip, &serv_addr.sin6_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    // connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    write(sock , hello , strlen(hello));	// send hello msg to server
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);	// read msg from server
    buffer[valread] = '\0';
    printf("%s\n",buffer );
    return 0;
}
