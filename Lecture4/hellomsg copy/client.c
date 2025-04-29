// Client side C/C++ program to demonstrate Socket programming
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
    // Create an int for socket and value read
    int sock, valread;
    // Create a sockaddr_in structure for the server
    struct sockaddr_in serv_addr;
    // Send a message from client
    char* msg = "Hello from client";
    // Set a buffer to be all 0's
    char buffer[1024] = {0};
    // Create a socket for the server
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failure");
        exit(EXIT_FAILURE);
    }
    // Set the server address to all 0's
    memset(&serv_addr, 0, sizeof(serv_addr));
    // Set the socket family to AF_INET and port to the specified port
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IP addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / address not supported");
        exit(EXIT_FAILURE);
    }
    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
        perror("Connect error");
        exit(EXIT_FAILURE);
    }
    // Write the hello message to the server
    write(sock, msg, sizeof(msg));
    printf("Hello message sent\n");
    // Read the message sent from the server
    read(sock, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    // Return 0
    return 0;
    
}
