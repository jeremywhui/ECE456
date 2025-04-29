// Server side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    // Create integers for server file descriptor, new socket, and value read
    int serverfd, newsocket, valread;
    // Create an internet socket address to store the client
    struct sockaddr_in address;
    // Set the address length to be the size of the socket address structure
    int addrlen = sizeof(address);
    // Set the buffer to all 0's
    char buffer[1024] = {0};
    // Create a string "Hello from server"
    char *hellomsg = "Hello from server";
      
    // Creating socket file descriptor
    if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failure");
        exit(EXIT_FAILURE);
    }
    // Erase the data in memory in the address structure
    memset(&address, 0, addrlen);
    // Set the address family to be AF_INET
    address.sin_family = AF_INET;
    // Set the IP address to be any address
    address.sin_addr.s_addr = INADDR_ANY;
    // Set the port to be in network-byte order
    address.sin_port = htons(PORT);
      
    // Forcefully attaching socket to the port 8080
    if (bind(serverfd, (struct sockaddr *) &address, addrlen) < 0) {
        perror("bind failure");
        exit(EXIT_FAILURE);
    } 
    // Listen on the server file descriptor, with 1024 connections
    if (listen(serverfd, 1024) < 0) {
        perror("listen failure");
        exit(EXIT_FAILURE);
    }
    // Block until the client is ready
    if ((newsocket = accept(serverfd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failure");
        exit(EXIT_FAILURE);
    }
    // Read the requests from the client and print it
    valread = read(newsocket, buffer, 1024);
    printf("%s\n", buffer);
    // Send the hello message to the server and print confirmation
    write(newsocket, hellomsg, strlen(buffer));
    printf("Hello message sent.\n");
    // Return 0
    return 0;
}
