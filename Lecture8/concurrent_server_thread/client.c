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
    struct sockaddr_in address;
    int sock1, sock2, valread;
    struct sockaddr_in serv_addr;
    char *hello1 = "Hello from client 1";
    char *hello2 = "Hello from client 2";
    char buffer1[1024] = {0};
    char buffer2[1024] = {0};
    if ((sock1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IP addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock1, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    if (connect(sock2, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    } 
    // Now the server serves both clients

    write(sock1 , hello1 , strlen(hello1));
    printf("Hello message sent from client 1\n");

    write(sock2 , hello2 , strlen(hello2));
    printf("Hello message sent from client 2\n");

    valread = read(sock1, buffer1, 1024); //receive msg from server
    printf("Client 1 received: %s\n", buffer1);
    valread = read(sock2, buffer2, 1024); //receive msg from server
    printf("Client 2 received: %s\n", buffer2);
    return 0;
}
